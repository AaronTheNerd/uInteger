#include "uInt.hpp"
#include <chrono>
#include <iostream>

atn::uInt c, d;
void fibo_fast(uint64_t n, atn::uInt res[]) {
    if (n == 0) {
        res[0] = atn::ZERO;
        res[1] = atn::ONE;
        return;
    }
    fibo_fast(n >> 1, res);
    c = ((res[1] << 1) - res[0]) * res[0];
    d = (res[0] * res[0] + res[1] * res[1]);
    if (n % atn::TWO == atn::ZERO) {
        res[0] = c;
        res[1] = d;
    } else {
        res[0] = d;
        res[1] = c + d;
    }
}

void fibo_fast(uint64_t n) {
    atn::uInt res[2] = { 0 };
    fibo_fast(n, res);
    std::cout << res[0] << std::endl;
}

#define N 100000

int main(int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();
    fibo_fast(N);
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = stop - start;
    std::cout << std::endl << "N=" << N << ", Runtime: " << total.count() << "ms" << std::endl;
    #if PERFORMANCE_TEST
    atn::print_performance_test_results();
    #endif
}