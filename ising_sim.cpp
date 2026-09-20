cat << 'EOF' > ising_model.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
void enable_virtual_terminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#else
void enable_virtual_terminal() {}
#endif

struct IsingModel {
    int L;
    int N;
    double J;
    double B;
    double T;
    std::vector<int> grid;

    std::mt19937 rng;
    std::uniform_real_distribution<double> dist_real;
    std::uniform_int_distribution<int> dist_idx;

    IsingModel(int size, double temp, double b_field, double j_coupling = 1.0)
        : L(size), N(size * size), T(temp), B(b_field), J(j_coupling),
          grid(size * size, 1), rng(1337),
          dist_real(0.0, 1.0), dist_idx(0, size - 1) {
        
        std::uniform_int_distribution<int> coin(0, 1);
        for (int i = 0; i < N; ++i) {
            grid[i] = coin(rng) ? 1 : -1;
        }
    }

    int get_spin(int x, int y) const {
        int px = (x + L) % L;
        int py = (y + L) % L;
        return grid[py * L + px];
    }

    void set_spin(int x, int y, int val) {
        int px = (x + L) % L;
        int py = (y + L) % L;
        grid[py * L + px] = val;
    }

    void metropolis_step() {
        for (int step = 0; step < N; ++step) {
            int x = dist_idx(rng);
            int y = dist_idx(rng);
            int s = get_spin(x, y);

            int neighbor_sum = get_spin(x + 1, y) +
                               get_spin(x - 1, y) +
                               get_spin(x, y + 1) +
                               get_spin(x, y - 1);

            double delta_E = 2.0 * s * (J * neighbor_sum + B);

            if (delta_E <= 0.0) {
                set_spin(x, y, -s);
            } else {
                double prob = std::exp(-delta_E / T);
                if (dist_real(rng) < prob) {
                    set_spin(x, y, -s);
                }
            }
        }
    }

    double compute_magnetization() const {
        int total = 0;
        for (int s : grid) total += s;
        return static_cast<double>(total) / N;
    }

    void draw(int step_count) const {
        std::cout << "\033[H";

        std::cout << "====================================\033[K\n";
        std::cout << "        2D ISING MODEL (METROPOLIS) \033[K\n";
        std::cout << "====================================\033[K\n";
        std::cout << " MCS: " << step_count 
                  << " | Temp: " << T 
                  << " | Magnetization: " << compute_magnetization() << "\033[K\n";
        std::cout << "------------------------------------\033[K\n";

        for (int y = 0; y < L; ++y) {
            std::cout << " ";
            for (int x = 0; x < L; ++x) {
                int s = get_spin(x, y);
                if (s == 1) {
                    // Mavi zemin + Beyaz blok (Spin +1)
                    std::cout << "\033[44;37m██\033[0m";
                } else {
                    // Kırmızı zemin + Nokta (Spin -1)
                    std::cout << "\033[41;30m··\033[0m";
                }
            }
            std::cout << "\033[K\n";
        }
        std::cout << "====================================\033[K\n";
        std::cout << std::flush;
    }
};

int main() {
    enable_virtual_terminal();
    std::cout << "\033[2J";

    // 28x28 kafes, T = 2.1 (Onsager Tc ≈ 2.269'un hemen altı, adalanma çok net izlenir)
    IsingModel sim(28, 2.1, 0.0);

    for (int mcs = 0; mcs < 600; ++mcs) {
        sim.draw(mcs);
        sim.metropolis_step();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0;
}
EOF

g++ -O3 -std=c++17 ising_model.cpp -o ising_sim
./ising_sim
