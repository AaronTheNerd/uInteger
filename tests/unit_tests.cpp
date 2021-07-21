#include "../src/uInt.hpp"
#include <bitset>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <time.h>


#define TEST(title, n, actual, expected, edge_case) \
    if (((actual) != (expected)) && !(edge_case)) { \
        std::cout << title << " (" << n << ") FAILED: expected = " << (expected) << ", actual = " << (actual) << std::endl; \
        result = false; \
    }

std::string to_binary_string(uint64_t n) {
    std::string result("");
    while (n != 0) {
        result = (n & 1 ? '1' : '0') + result;
        n >>= 1;
    }
    return result;
}

std::string to_base64_string(uint64_t n) {
    std::string result("");
    uint64_t mask = 0x3F;
    while (n != 0) {
        result = atn::base64_index[n & mask] + result;
        n >>= 6;
    }
    return result;
}

bool test_int_init(uint64_t n) {
    bool result = true;
    atn::uInt u = n;
    TEST("TEST_INT_INIT", 1, uint64_t(u), n, false)
    return result;
}

bool test_decimal_string_init(uint64_t n) {
    bool result = true;
    atn::uInt u(std::to_string(n));
    TEST("TEST_DECIMAL_STRING_INIT", 1, uint64_t(u), n, false)
    return result;
}

bool test_hexadecimal_string_init(uint64_t n) {
    bool result = true;
    std::stringstream stream;
    stream << std::hex << n;
    atn::uInt u("0x" + stream.str());
    TEST("TEST_HEXADECIMAL_STRING_INIT", 1, uint64_t(u), n, false)
    u = atn::uInt("0X" + stream.str());
    TEST("TEST_HEXADECIMAL_STRING_INIT", 2, uint64_t(u), n, false)
    return result;
}

bool test_binary_string_init(uint64_t n) {
    bool result = true;
    std::stringstream stream;
    stream << std::bitset<64>(n);
    atn::uInt u("0b" + stream.str());
    TEST("TEST_BINARY_STRING_INIT", 1, uint64_t(u), n, false)
    u = atn::uInt("0B" + stream.str());
    TEST("TEST_BINARY_STRING_INIT", 2, uint64_t(u), n, false)
    return result;
}

bool test_uint_to_int(uint64_t n) {
    bool result = true;
    atn::uInt u = n;
    TEST("TEST_UINT_TO_INT", 1, uint64_t(u), n, false)
    return result;
}

bool test_add(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_ADD", 1, uint64_t(u1 + u2), n1 + n2, false)
    TEST("TEST_ADD", 2, uint64_t(u2 + u1), n2 + n1, false)
    TEST("TEST_ADD", 3, uint64_t(u1 + 1), n1 + 1, false)
    TEST("TEST_ADD", 4, uint64_t(u2 + 1), n2 + 1, false)
    TEST("TEST_ADD", 5, uint64_t(u1 + 0), n1 + 0, false)
    TEST("TEST_ADD", 6, uint64_t(u2 + 0), n2 + 0, false)
    return result;
}

bool test_sub(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_SUB", 1, uint64_t(u1 - u2), n1 - n2, uint64_t(u1 - u2) == 0 && n2 > n1)
    TEST("TEST_SUB", 2, uint64_t(u2 - u1), n2 - n1, uint64_t(u2 - u1) == 0 && n1 > n2)
    TEST("TEST_SUB", 3, uint64_t(u1 - 1), n1 - 1, false)
    TEST("TEST_SUB", 4, uint64_t(u2 - 1), n2 - 1, false)
    TEST("TEST_SUB", 5, uint64_t(u1 - 0), n1 - 0, false)
    TEST("TEST_SUB", 6, uint64_t(u2 - 0), n2 - 0, false)
    return result;
}

bool test_mul(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_MUL", 1, uint64_t(u1 * u2), n1 * n2, false)
    TEST("TEST_MUL", 2, uint64_t(u2 * u1), n2 * n1, false)
    TEST("TEST_MUL", 3, uint64_t(u1 * 1), n1 * 1, false)
    TEST("TEST_MUL", 4, uint64_t(u2 * 1), n2 * 1, false)
    TEST("TEST_MUL", 5, uint64_t(u1 * 0), n1 * 0, false)
    TEST("TEST_MUL", 6, uint64_t(u2 * 0), n2 * 0, false)
    return result;
}

bool test_div(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_DIV", 1, uint64_t(u1 / u2), n1 / n2, false)
    TEST("TEST_DIV", 2, uint64_t(u2 / u1), n2 / n1, false)
    TEST("TEST_DIV", 3, uint64_t(u1 / 1), n1 / 1, false)
    TEST("TEST_DIV", 4, uint64_t(u2 / 1), n2 / 1, false)
    TEST("TEST_DIV", 5, uint64_t(0 / u1), 0 / n1, false)
    TEST("TEST_DIV", 6, uint64_t(0 / u2), 0 / n2, false)
    return result;
}

bool test_mod(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_MOD", 1, uint64_t(u1 % u2), n1 % n2, false)
    TEST("TEST_MOD", 2, uint64_t(u2 % u1), n2 % n1, false)
    TEST("TEST_MOD", 3, uint64_t(u1 % 1), n1 % 1, false)
    TEST("TEST_MOD", 4, uint64_t(u2 % 1), n2 % 1, false)
    TEST("TEST_MOD", 5, uint64_t(0 % u1), 0 % n1, false)
    TEST("TEST_MOD", 6, uint64_t(0 % u2), 0 % n2, false)
    return result;
}

bool test_shift_left(uint64_t n) {
    bool result = true;
    atn::uInt u = n;
    TEST("TEST_SL", 1, uint64_t(u << 0), n << 0, false)
    TEST("TEST_SL", 2, uint64_t(u << 1), n << 1, false)
    TEST("TEST_SL", 3, uint64_t(u << 2), n << 2, false)
    return result;
}

bool test_shift_right(uint64_t n) {
    bool result = true;
    atn::uInt u = n;
    TEST("TEST_SR", 1, uint64_t(u >> 0), n >> 0, false)
    TEST("TEST_SR", 2, uint64_t(u >> 1), n >> 1, false)
    TEST("TEST_SR", 3, uint64_t(u >> 2), n >> 2, false)
    return result;
}

bool test_and(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_AND", 1, uint64_t(u1 & u2), n1 & n2, false)
    TEST("TEST_AND", 2, uint64_t(u2 & u1), n2 & n1, false)
    TEST("TEST_AND", 3, uint64_t(u1 & 1), n1 & 1, false)
    TEST("TEST_AND", 4, uint64_t(u2 & 1), n2 & 1, false)
    TEST("TEST_AND", 5, uint64_t(u1 & 0), n1 & 0, false)
    TEST("TEST_AND", 6, uint64_t(u2 & 0), n2 & 0, false)
    return result;
}

bool test_or(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_OR", 1, uint64_t(u1 | u2), n1 | n2, false)
    TEST("TEST_OR", 2, uint64_t(u2 | u1), n2 | n1, false)
    TEST("TEST_OR", 3, uint64_t(u1 | 1), n1 | 1, false)
    TEST("TEST_OR", 4, uint64_t(u2 | 1), n2 | 1, false)
    TEST("TEST_OR", 5, uint64_t(u1 | 0), n1 | 0, false)
    TEST("TEST_OR", 6, uint64_t(u2 | 0), n2 | 0, false)
    return result;
}

bool test_xor(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_XOR", 1, uint64_t(u1 ^ u2), n1 ^ n2, false)
    TEST("TEST_XOR", 2, uint64_t(u2 ^ u1), n2 ^ n1, false)
    TEST("TEST_XOR", 3, uint64_t(u1 ^ 1), n1 ^ 1, false)
    TEST("TEST_XOR", 4, uint64_t(u2 ^ 1), n2 ^ 1, false)
    TEST("TEST_XOR", 5, uint64_t(u1 ^ 0), n1 ^ 0, false)
    TEST("TEST_XOR", 6, uint64_t(u2 ^ 0), n2 ^ 0, false)
    return result;
}

bool test_eq(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_EQ", 1, u1 == u2, n1 == n2, false)
    TEST("TEST_EQ", 2, u2 == u1, n2 == n1, false)
    TEST("TEST_EQ", 3, u1 == u1, n1 == n1, false)
    TEST("TEST_EQ", 4, u2 == u2, n2 == n2, false)
    return result;
}

bool test_neq(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_NEQ", 1, u1 != u2, n1 != n2, false)
    TEST("TEST_NEQ", 2, u2 != u1, n2 != n1, false)
    TEST("TEST_NEQ", 3, u1 != u1, n1 != n1, false)
    TEST("TEST_NEQ", 4, u2 != u2, n2 != n2, false)
    return result;
}

bool test_lt(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_LT", 1, u1 < u2, n1 < n2, false)
    TEST("TEST_LT", 2, u1 < u2, n1 < n2, false)
    TEST("TEST_LT", 3, u1 < u2, n1 < n2, false)
    TEST("TEST_LT", 4, u1 < u2, n1 < n2, false)
    return result;
}

bool test_gt(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_GT", 1, u1 > u2, n1 > n2, false)
    TEST("TEST_GT", 2, u1 > u2, n1 > n2, false)
    TEST("TEST_GT", 3, u1 > u2, n1 > n2, false)
    TEST("TEST_GT", 4, u1 > u2, n1 > n2, false)
    return result;
}

bool test_lte(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_LTE", 1, u1 <= u2, n1 <= n2, false)
    TEST("TEST_LTE", 2, u2 <= u1, n2 <= n1, false)
    TEST("TEST_LTE", 3, u1 <= u1, n1 <= n1, false)
    TEST("TEST_LTE", 4, u2 <= u2, n2 <= n2, false)
    return result;
}

bool test_gte(uint64_t n1, uint64_t n2) {
    bool result = true;
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_GTE", 1, u1 >= u2, n1 >= n2, false)
    TEST("TEST_GTE", 2, u2 >= u1, n2 >= n1, false)
    TEST("TEST_GTE", 3, u1 >= u1, n1 >= n1, false)
    TEST("TEST_GTE", 4, u2 >= u2, n2 >= n2, false)
    return result;
}

bool test_to_string(uint64_t n) {
    atn::uInt u = n;
    std::stringstream stream;
    bool result = true;
    TEST("TEST_TO_STRING", 1, u.to_string(), std::to_string(n), false)
    TEST("TEST_TO_STRING", 2, u.to_string(2), to_binary_string(n), false)
    stream << std::oct << n;
    TEST("TEST_TO_STRING", 3, u.to_string(8), stream.str(), false)
    stream.str(std::string());
    stream << std::hex << n;
    TEST("TEST_TO_STRING", 4, u.to_string(16), stream.str(), false)
    TEST("TEST_TO_STRING", 5, u.to_string(64), to_base64_string(n), false)
    return result;
}

bool test_all(uint64_t n1, uint64_t n2) {
    bool result = true;
    result &= test_int_init(n1);
    result &= test_int_init(n2);
    result &= test_decimal_string_init(n1);
    result &= test_decimal_string_init(n2);
    result &= test_hexadecimal_string_init(n1);
    result &= test_hexadecimal_string_init(n2);
    result &= test_binary_string_init(n1);
    result &= test_binary_string_init(n2);
    result &= test_uint_to_int(n1);
    result &= test_uint_to_int(n2);
    result &= test_add(n1, n2);
    result &= test_sub(n1, n2);
    result &= test_mul(n1, n2);
    result &= test_div(n1, n2);
    result &= test_mod(n1, n2);
    result &= test_shift_left(n1);
    result &= test_shift_left(n2);
    result &= test_shift_right(n1);
    result &= test_shift_right(n2);
    result &= test_and(n1, n2);
    result &= test_or(n1, n2);
    result &= test_xor(n1, n2);
    result &= test_eq(n1, n2);
    result &= test_neq(n1, n2);
    result &= test_lt(n1, n2);
    result &= test_gt(n1, n2);
    result &= test_lte(n1, n2);
    result &= test_gte(n1, n2);
    result &= test_to_string(n1);
    result &= test_to_string(n2);
    if (!result) {
        std::cout << "Some test cases failed" << std::endl;
    } else {
        std::cout << "All test cases succeeded" << std::endl;
    }
    return result;
}

int main(int argc, char** argv) {
    uint64_t N = 100;
    if (argc > 1) {
        N = std::stoi(argv[1]);
    }
    uint64_t SEED = time(NULL);
    if (argc > 2) {
        SEED = std::stoi(argv[2]);
    }
    srand(SEED);
    std::cout << "SEED=" << SEED << ", N=" << N << std::endl << std::endl;
    std::chrono::duration<double, std::milli> total;
    auto start = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < N; ++i) {
        uint64_t rand1 = rand();
        uint64_t rand2 = rand();
        std::cout << "=======================================" << std::endl;
        std::cout << "RUN #" << (i + 1) << ", n1=" << rand1 << ", n2=" << rand2 << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        bool result = test_all(rand1, rand2);
        auto stop = std::chrono::high_resolution_clock::now();
        total += (stop - start);
        std::cout << "=======================================" << std::endl << std::endl;
        if (!result) {
            throw std::runtime_error("TEST #" + std::to_string(i + 1) + " HAS FAILED");
        }
    }
    std::cout << "ALL TESTS SUCCEEDED" << std::endl << std::endl;
    std::cout << "Average time per test run: " << total.count() / N << "ms" << std::endl;
    #ifdef PERFORMANCE_TEST
        atn::print_performance_test_results();
    #endif
}