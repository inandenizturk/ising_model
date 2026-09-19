

# 1D Ising Ring Model (Periodic Boundary Conditions)

A high-performance C++ implementation of the 1D Ising Model on a ring topology using the Metropolis Monte Carlo algorithm.

## Features
- **Periodic Boundary Conditions (Ring):** Circular lattice topology avoiding surface edge effects.
- **Metropolis Algorithm:** Stochastic energy minimization driven by Boltzmann probabilities.
- **Pure Modern C++:** Fast execution with zero external graphical dependencies.

## Compilation & Run
```bash
clang++ -std=c++17 -O3 ising_ring.cpp -o ising_ring
./ising_ring


MIT License

Copyright (c) 2026 İnan Deniz Türk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
