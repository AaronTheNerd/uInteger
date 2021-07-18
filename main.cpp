#include "uInt.hpp"
#include <cstdlib>
#include <chrono>
#include <iostream>
void test() {
    srand(0);
    uint64_t rand1 = rand();
    uint64_t rand2 = rand();
    std::cout << rand1 << ", " << rand2 << std::endl;
    atn::uInt a = rand1; // 1804289383 (GOOD)
    std::cout << a << std::endl;
    atn::uInt b = rand2; // 846930886 (GOOD)
    std::cout << b << std::endl;
    atn::uInt c = a + b; // 2651220269 (GOOD)
    std::cout << c << std::endl;
    atn::uInt d = a - b; // 957358497 (GOOD)
    std::cout << d << std::endl;
    atn::uInt e = a * b; // 1.528e18 (GOOD)
    std::cout << e << std::endl;
    atn::uInt f = e / b; // 1804289383 (GOOD)
    std::cout << f << std::endl;
    atn::uInt g = a % b; // 110427611 (GOOD)
    std::cout << g << std::endl;
    atn::uInt h = b - a; // 0
    std::cout << h << std::endl;
    atn::uInt i("0xBABEFEEDDEADBEEF"); // 13456473034226253551
    std::cout << i << std::endl;
    atn::uInt j("31415926535897932384626433"); // 31415926535897932384626433
    std::cout << j << std::endl;
    atn::uInt k = a & b; // 571146566
    std::cout << k << std::endl;
    atn::uInt l = a | b; // 2080073703
    std::cout << l << std::endl;
    atn::uInt m = a ^ b; // 1508927137
    std::cout << m << std::endl;
}
void test_iter(uint64_t n) {
    atn::uInt curr;
    for (uint64_t i = 0; i < n; ++i)
        std::cout << ++curr << std::endl;
    for (uint64_t i = 0; i < n; ++i)
        std::cout << --curr << std::endl;
}
void fibo(uint64_t n) {
    // I was always under the impression that this was one of the best ways to
    // calculate the fibonacci sequence. Since this is O(n) time complexity and
    // O(1) space.
    atn::uInt arr[2];
    arr[0] = 0;
    arr[1] = 1;
    bool curr = false;
    if (n < 2) {
        std::cout << arr[n] << std::endl;
        return;
    }
    for (uint64_t i = 1; i < n; ++i) {
        arr[curr] += arr[!curr];
        curr = !curr;
    }
    std::cout << arr[!curr] << std::endl;
}
atn::uInt a, b, c, d;
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
#define N 1000
int main(int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();
    fibo_fast(N);
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> total = stop - start;
    std::cout << std::endl << "N=" << N << ", Runtime: " << total.count() << "ms" << std::endl;
    //test();
    atn::print_performance_test_results();
}