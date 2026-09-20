cat << 'EOF' > ising_cyberpunk.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <thread>
#include <chrono>
#include <iomanip>
#include <csignal>
#include <deque>

// Terminali temiz kapatma yakalayıcısı
void signal_handler(int) {
    std::cout << "\033[?25h\033[0m\n\n[Simulation Terminated Cleanly]\n";
    std::exit(0);
}

// 24-bit True-Color ANSI Renkleri
const std::string RESET      = "\033[0m";
const std::string BORDER     = "\033[38;2;0;200;220m";   // Neon Cyan
const std::string TEXT_MUTED = "\033[38;2;120;140;150m"; // HUD Açık Gri
const std::string TEXT_BRIGHT= "\033[38;2;240;255;255m"; // Saf Beyaz
const std::string STAT_VAL   = "\033[38;2;0;255;200m";   // Neon Nane Yeşili
const std::string SPIN_UP    = "\033[48;2;0;240;255m  " + RESET; // Elektrik Cyan Blok
const std::string SPIN_DOWN  = "\033[48;2;255;0;85m  " + RESET;  // Neon Magenta Blok

class CyberIsing {
private:
    int L, N;
    double T, B, J;
    std::vector<int> grid;
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist_real;
    std::uniform_int_distribution<int> dist_idx;

    long long accepted_flips = 0;
    long long total_trials = 0;

    std::deque<double> mag_history;
    std::deque<double> energy_history;
    const size_t SPARK_LEN = 20;

    // Unicode Sparkline karakterleri
    const std::vector<std::string> sparks = {" ", "▂", "▃", "▄", "▅", "▆", "▇", "█"};

public:
    CyberIsing(int size, double temp, double field = 0.0, double coupling = 1.0)
        : L(size), N(size * size), T(temp), B(field), J(coupling),
          grid(size * size), rng(1337), dist_real(0.0, 1.0), dist_idx(0, size - 1) {
        for (int i = 0; i < N; ++i) {
            grid[i] = (dist_real(rng) < 0.5) ? -1 : 1;
        }
    }

    int get(int x, int y) const {
        return grid[((y + L) % L) * L + ((x + L) % L)];
    }

    void metropolis_sweep() {
        long long current_accepted = 0;
        for (int step = 0; step < N; ++step) {
            int x = dist_idx(rng);
            int y = dist_idx(rng);
            int s = grid[y * L + x];

            int nn = get(x + 1, y) + get(x - 1, y) + get(x, y + 1) + get(x, y - 1);
            double dE = 2.0 * s * (J * nn + B);

            if (dE <= 0.0 || dist_real(rng) < std::exp(-dE / T)) {
                grid[y * L + x] = -s;
                current_accepted++;
            }
        }
        accepted_flips += current_accepted;
        total_trials += N;

        // Geçmiş verileri güncelle
        mag_history.push_back(std::abs(compute_magnetization()));
        if (mag_history.size() > SPARK_LEN) mag_history.pop_front();

        energy_history.push_back(compute_energy_per_spin());
        if (energy_history.size() > SPARK_LEN) energy_history.pop_front();
    }

    double compute_magnetization() const {
        int sum = 0;
        for (int s : grid) sum += s;
        return static_cast<double>(sum) / N;
    }

    double compute_energy_per_spin() const {
        double E = 0.0;
        for (int y = 0; y < L; ++y) {
            for (int x = 0; x < L; ++x) {
                int s = grid[y * L + x];
                int nn = grid[y * L + ((x + 1) % L)] + grid[((y + 1) % L) * L + x];
                E -= J * s * nn;
                E -= B * s;
            }
        }
        return E / N;
    }

    std::string generate_sparkline(const std::deque<double>& data, double min_v, double max_v) const {
        if (data.empty()) return "";
        std::string res;
        for (double v : data) {
            double norm = (max_v > min_v) ? (v - min_v) / (max_v - min_v) : 0.5;
            norm = std::clamp(norm, 0.0, 0.999);
            int idx = static_cast<int>(norm * sparks.size());
            res += sparks[idx];
        }
        return res;
    }

    void render(int mcs) {
        std::cout << "\033[H"; // Cursor home
        double M = compute_magnetization();
        double E = compute_energy_per_spin();
        double acc_rate = (total_trials > 0) ? (100.0 * accepted_flips / total_trials) : 0.0;

        // Başlık
        std::cout << BORDER << "╔══════════════════════════════════════════════════════════════════════════════════╗" << RESET << "\n";
        std::cout << BORDER << "║   " << TEXT_BRIGHT << "2D ISING FERROMAGNETIC SIMULATION [METROPOLIS ENGINE]                          " << BORDER << "║" << RESET << "\n";
        std::cout << BORDER << "╠════════════════════════════════════════════════════════╦═════════════════════════╣" << RESET << "\n";

        // Kafes ve Yan Panel HUD
        for (int y = 0; y < L; ++y) {
            std::cout << BORDER << "║ " << RESET;
            for (int x = 0; x < L; ++x) {
                std::cout << (grid[y * L + x] == 1 ? SPIN_UP : SPIN_DOWN);
            }
            std::cout << BORDER << " ║" << RESET;

            // Yan panel telemetri satırları
            if (y == 1)  std::cout << TEXT_MUTED  << "  [ TELEMETRY STATUS ]   " << BORDER << "║";
            else if (y == 2)  std::cout << TEXT_MUTED  << "  MCS    : " << STAT_VAL << std::setw(8) << mcs << "      " << BORDER << "║";
            else if (y == 3)  std::cout << TEXT_MUTED  << "  Temp(T): " << STAT_VAL << std::fixed << std::setprecision(2) << std::setw(6) << T << " (Tc≈2.27) " << BORDER << "║";
            else if (y == 4)  std::cout << TEXT_MUTED  << "  Field B: " << STAT_VAL << std::setw(6) << B << "        " << BORDER << "║";
            else if (y == 5)  std::cout << TEXT_MUTED  << "  Accept : " << STAT_VAL << std::setprecision(1) << std::setw(5) << acc_rate << "%       " << BORDER << "║";
            else if (y == 7)  std::cout << TEXT_MUTED  << "  [ OBSERVABLES ]        " << BORDER << "║";
            else if (y == 8)  std::cout << TEXT_MUTED  << "  Mag |M|: " << STAT_VAL << std::setprecision(4) << std::setw(7) << std::abs(M) << "        " << BORDER << "║";
            else if (y == 9)  std::cout << TEXT_MUTED  << "  Trend  : " << "\033[38;2;0;240;255m" << std::left << std::setw(15) << generate_sparkline(mag_history, 0.0, 1.0) << std::right << RESET << BORDER << "║";
            else if (y == 11) std::cout << TEXT_MUTED  << "  Energy : " << STAT_VAL << std::setprecision(3) << std::setw(7) << E << "        " << BORDER << "║";
            else if (y == 12) std::cout << TEXT_MUTED  << "  Trend  : " << "\033[38;2;255;0;85m" << std::left << std::setw(15) << generate_sparkline(energy_history, -2.0, 0.0) << std::right << RESET << BORDER << "║";
            else if (y == 14) std::cout << TEXT_MUTED  << "  [ LEGEND ]             " << BORDER << "║";
            else if (y == 15) std::cout << "  " << SPIN_UP << TEXT_MUTED << " Spin +1 (Cyan)   " << BORDER << "║";
            else if (y == 16) std::cout << "  " << SPIN_DOWN << TEXT_MUTED << " Spin -1 (Pink)   " << BORDER << "║";
            else if (y == 18) std::cout << TEXT_MUTED  << "  Press Ctrl+C to exit   " << BORDER << "║";
            else              std::cout << "                         " << BORDER << "║";

            std::cout << "\n";
        }

        std::cout << BORDER << "╚════════════════════════════════════════════════════════╩═════════════════════════╝" << RESET << "\n";
        std::cout << std::flush;
    }
};

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signal_handler);

    double temp = 2.15; // Kritik eşiğin hemen altı (fraktal neon kümeleri)
    double field = 0.0;
    if (argc > 1) temp = std::stod(argv[1]);
    if (argc > 2) field = std::stod(argv[2]);

    std::cout << "\033[2J\033[?25l"; // Ekranı sil, imleci gizle

    CyberIsing sim(26, temp, field);

    for (int mcs = 1; mcs <= 2500; ++mcs) {
        sim.metropolis_sweep();
        sim.render(mcs);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    std::cout << "\033[?25h\033[0m\n";
    return 0;
}
EOF

clang++ -std=c++20 -O3 ising_cyberpunk.cpp -o ising_cyberpunk
./ising_cyberpunk
