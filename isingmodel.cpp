cat << 'EOF' > ising_terminal.cpp
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
#include <thread>
#include <iomanip>

const int L = 30;
const double J = 1.0;
const int TOTAL_MCS = 2000;
double T = 2.15;
std::vector<int> grid(L * L);
std::mt19937 rng(1337);
std::uniform_real_distribution<double> dist_real(0.0, 1.0);
std::uniform_int_distribution<int> dist_site(0, L * L - 1);

void init_grid() {
    for (int i = 0; i < L * L; ++i) grid[i] = (dist_real(rng) < 0.5) ? -1 : 1;
}

void metropolis_step() {
    for (int step = 0; step < L * L; ++step) {
        int idx = dist_site(rng);
        int x = idx % L, y = idx / L;
        int s = grid[idx];
        int up = grid[((y - 1 + L) % L) * L + x];
        int down = grid[((y + 1) % L) * L + x];
        int left = grid[y * L + ((x - 1 + L) % L)];
        int right = grid[y * L + ((x + 1) % L)];
        double dE = 2.0 * J * s * (up + down + left + right);
        if (dE <= 0.0 || dist_real(rng) < std::exp(-dE / T)) grid[idx] = -s;
    }
}

double compute_magnetization() {
    double total = 0.0;
    for (int s : grid) total += s;
    return total / (L * L);
}

void render_terminal(int mcs, double mag) {
    std::cout << "\033[H";
    std::cout << "\033[1;36m┌────────────────────────────────────────────────────────┐\033[0m\n";
    std::cout << "\033[1;36m│       2D ISING FERROMAGNETIC SIMULATION (METROPOLIS)   │\033[0m\n";
    std::cout << "\033[1;36m└────────────────────────────────────────────────────────┘\033[0m\n";
    std::cout << "\033[1;32mMCS: " << std::setw(4) << mcs 
              << " \033[0m| \033[1;34mTemp (T): " << std::fixed << std::setprecision(2) << T 
              << " \033[0m| \033[1;32mMag (M): " << std::setprecision(6) << mag << "\033[0m\n\n";

    for (int y = 0; y < L; ++y) {
        for (int x = 0; x < L; ++x) {
            std::cout << (grid[y * L + x] == 1 ? "\033[41m  \033[0m" : "\033[46m  \033[0m");
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

int main(int argc, char* argv[]) {
    if (argc > 1) T = std::stod(argv[1]);
    std::cout << "\033[2J\033[?25l";
    init_grid();
    for (int mcs = 1; mcs <= TOTAL_MCS; ++mcs) {
        metropolis_step();
        if (mcs % 2 == 0) {
            render_terminal(mcs, compute_magnetization());
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
    }
    std::cout << "\033[?25h\n";
    return 0;
}
EOF
clang++ -std=c++17 -O3 ising_terminal.cpp -o ising_sim
./ising_sim
