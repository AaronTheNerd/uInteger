#include "../src/uInt.hpp"
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

atn::uInt fibo_fast(uint64_t n) {
    atn::uInt res[2] = { 0 };
    fibo_fast(n, res);
    return res[0];
}

int main(int argc, char** argv) {
    uint64_t N = 100;
    if (argc > 1) {
        N = std::stoi(argv[1]);
    }
    auto start = std::chrono::high_resolution_clock::now();
    atn::uInt n = fibo_fast(N);
    std::string str = n.to_string();
    std::cout << str << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = stop - start;
    std::cout << std::endl << "N=" << N << ", Runtime: " << total.count() << "ms" << std::endl;
    std::cout << "Decimal digits: " << str.size() << ", Binary digits: " << n.bits.size() << std::endl;
    #if PERFORMANCE_TEST
    atn::print_performance_test_results();
    #endif
}