#include "../src/uInt.hpp"
#include <bitset>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>
#include <time.h>


#define TEST(title, n, actual, expected) \
    if ((actual) != (expected)) { \
        std::cout << title << " (" << n << ") FAILED: expected = " << (expected) << ", actual = " << (actual) << std::endl; \
        return false; \
    }


bool test_int_init(uint64_t n) {
    atn::uInt u = n;
    TEST("TEST_INT_INIT", 1, u.to_string(), std::to_string(n))
    return true;
}

bool test_decimal_string_init(uint64_t n) {
    std::string str = std::to_string(n);
    atn::uInt u(str);
    TEST("TEST_DECIMAL_STRING_INIT", 1, u.to_string(), str)
    return true;
}

bool test_hexadecimal_string_init(uint64_t n) {
    std::stringstream stream;
    stream << std::hex << n;
    atn::uInt u("0x" + stream.str());
    TEST("TEST_HEXADECIMAL_STRING_INIT", 1, u.to_string(), std::to_string(n))
    u = atn::uInt("0X" + stream.str());
    TEST("TEST_HEXADECIMAL_STRING_INIT", 2, u.to_string(), std::to_string(n))
    return true;
}

bool test_binary_string_init(uint64_t n) {
    std::stringstream stream;
    stream << std::bitset<64>(n);
    atn::uInt u("0b" + stream.str());
    TEST("TEST_BINARY_STRING_INIT", 1, u.to_string(), std::to_string(n))
    u = atn::uInt("0B" + stream.str());
    TEST("TEST_BINARY_STRING_INIT", 2, u.to_string(), std::to_string(n))
    return true;
}

bool test_add(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_ADD", 1, (u1 + u2).to_string(), std::to_string(n1 + n2))
    TEST("TEST_ADD", 2, (u2 + u1).to_string(), std::to_string(n2 + n1))
    TEST("TEST_ADD", 3, (u1 + 1).to_string(), std::to_string(n1 + 1))
    TEST("TEST_ADD", 4, (u2 + 1).to_string(), std::to_string(n2 + 1))
    TEST("TEST_ADD", 5, (u1 + 0).to_string(), std::to_string(n1 + 0))
    TEST("TEST_ADD", 6, (u2 + 0).to_string(), std::to_string(n2 + 0))
    return true;
}

bool test_sub(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    if ((u1 - u2).to_string() != std::to_string(n1 - n2) && !((u1 - u2).to_string() == std::string("0") && n2 > n1)) {
        std::cout << "TEST_SUB (1) FAILED: expected = " << (n1 - n2) << ", actual = " << (u1 - u2).to_string() << std::endl;
        return false;
    }
    if ((u2 - u1).to_string() != std::to_string(n2 - n1) && !((u2 - u1).to_string() == std::string("0") && n1 > n2)) {
        std::cout << "TEST_SUB (2) FAILED: expected = " << (n2 - n1) << ", actual = " << (u2 - u1).to_string() << std::endl;
        return false;
    }
    if ((u1 - 1).to_string() != std::to_string(n1 - 1)) {
        std::cout << "TEST_SUB (3) FAILED: expected = " << (n1 - 1) << ", actual = " << (u1 - 1).to_string() << std::endl;
        return false;
    }
    if ((u2 - 1).to_string() != std::to_string(n2 - 1)) {
        std::cout << "TEST_SUB (4) FAILED: expected = " << (n2 - 1) << ", actual = " << (u2 - 1).to_string() << std::endl;
        return false;
    }
    if ((u1 - 0).to_string() != std::to_string(n1 - 0)) {
        std::cout << "TEST_SUB (5) FAILED: expected = " << (n1 - 0) << ", actual = " << (u1 - 0).to_string() << std::endl;
        return false;
    }
    if ((u2 - 0).to_string() != std::to_string(n2 - 0)) {
        std::cout << "TEST_SUB (6) FAILED: expected = " << (n2 - 0) << ", actual = " << (u2 - 0).to_string() << std::endl;
        return false;
    }
    return true;
}

bool test_mul(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_MUL", 1, (u1 * u2).to_string(), std::to_string(n1 * n2))
    TEST("TEST_MUL", 2, (u2 * u1).to_string(), std::to_string(n2 * n1))
    TEST("TEST_MUL", 3, (u1 * 1).to_string(), std::to_string(n1 * 1))
    TEST("TEST_MUL", 4, (u2 * 1).to_string(), std::to_string(n2 * 1))
    TEST("TEST_MUL", 5, (u1 * 0).to_string(), std::to_string(n1 * 0))
    TEST("TEST_MUL", 6, (u2 * 0).to_string(), std::to_string(n2 * 0))
    return true;
}

bool test_div(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_DIV", 1, (u1 / u2).to_string(), std::to_string(n1 / n2))
    TEST("TEST_DIV", 2, (u2 / u1).to_string(), std::to_string(n2 / n1))
    TEST("TEST_DIV", 3, (u1 / 1).to_string(), std::to_string(n1 / 1))
    TEST("TEST_DIV", 4, (u2 / 1).to_string(), std::to_string(n2 / 1))
    TEST("TEST_DIV", 5, (0 / u1).to_string(), std::to_string(0 / n1))
    TEST("TEST_DIV", 6, (0 / u2).to_string(), std::to_string(0 / n2))
    return true;
}

bool test_mod(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_MOD", 1, (u1 % u2).to_string(), std::to_string(n1 % n2))
    TEST("TEST_MOD", 2, (u2 % u1).to_string(), std::to_string(n2 % n1))
    TEST("TEST_MOD", 3, (u1 % 1).to_string(), std::to_string(n1 % 1))
    TEST("TEST_MOD", 4, (u2 % 1).to_string(), std::to_string(n2 % 1))
    TEST("TEST_MOD", 5, (0 % u1).to_string(), std::to_string(0 % n1))
    TEST("TEST_MOD", 6, (0 % u2).to_string(), std::to_string(0 % n2))
    return true;
}

bool test_shift_left(uint64_t n) {
    atn::uInt u = n;
    TEST("TEST_SL", 1, (u << 0).to_string(), std::to_string(n << 0))
    TEST("TEST_SL", 2, (u << 1).to_string(), std::to_string(n << 1))
    TEST("TEST_SL", 3, (u << 2).to_string(), std::to_string(n << 2))
    return true;
}

bool test_shift_right(uint64_t n) {
    atn::uInt u = n;
    TEST("TEST_SR", 1, (u >> 0).to_string(), std::to_string(n >> 0))
    TEST("TEST_SR", 2, (u >> 1).to_string(), std::to_string(n >> 1))
    TEST("TEST_SR", 3, (u >> 2).to_string(), std::to_string(n >> 2))
    return true;
}

bool test_and(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_AND", 1, (u1 & u2).to_string(), std::to_string(n1 & n2))
    TEST("TEST_AND", 2, (u2 & u1).to_string(), std::to_string(n2 & n1))
    TEST("TEST_AND", 3, (u1 & 1).to_string(), std::to_string(n1 & 1))
    TEST("TEST_AND", 4, (u2 & 1).to_string(), std::to_string(n2 & 1))
    TEST("TEST_AND", 5, (u1 & 0).to_string(), std::to_string(n1 & 0))
    TEST("TEST_AND", 6, (u2 & 0).to_string(), std::to_string(n2 & 0))
    return true;
}

bool test_or(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_OR", 1, (u1 | u2).to_string(), std::to_string(n1 | n2))
    TEST("TEST_OR", 2, (u2 | u1).to_string(), std::to_string(n2 | n1))
    TEST("TEST_OR", 3, (u1 | 1).to_string(), std::to_string(n1 | 1))
    TEST("TEST_OR", 4, (u2 | 1).to_string(), std::to_string(n2 | 1))
    TEST("TEST_OR", 5, (u1 | 0).to_string(), std::to_string(n1 | 0))
    TEST("TEST_OR", 6, (u2 | 0).to_string(), std::to_string(n2 | 0))
    return true;
}

bool test_xor(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_XOR", 1, (u1 ^ u2).to_string(), std::to_string(n1 ^ n2))
    TEST("TEST_XOR", 2, (u2 ^ u1).to_string(), std::to_string(n2 ^ n1))
    TEST("TEST_XOR", 3, (u1 ^ 1).to_string(), std::to_string(n1 ^ 1))
    TEST("TEST_XOR", 4, (u2 ^ 1).to_string(), std::to_string(n2 ^ 1))
    TEST("TEST_XOR", 5, (u1 ^ 0).to_string(), std::to_string(n1 ^ 0))
    TEST("TEST_XOR", 6, (u2 ^ 0).to_string(), std::to_string(n2 ^ 0))
    return true;
}

bool test_eq(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_EQ", 1, u1 == u2, n1 == n2)
    TEST("TEST_EQ", 2, u2 == u1, n2 == n1)
    TEST("TEST_EQ", 3, u1 == u1, n1 == n1)
    TEST("TEST_EQ", 4, u2 == u2, n2 == n2)
    return true;
}

bool test_neq(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_NEQ", 1, u1 != u2, n1 != n2)
    TEST("TEST_NEQ", 2, u2 != u1, n2 != n1)
    TEST("TEST_NEQ", 3, u1 != u1, n1 != n1)
    TEST("TEST_NEQ", 4, u2 != u2, n2 != n2)
    return true;
}

bool test_lt(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_LT", 1, u1 < u2, n1 < n2)
    TEST("TEST_LT", 2, u1 < u2, n1 < n2)
    TEST("TEST_LT", 3, u1 < u2, n1 < n2)
    TEST("TEST_LT", 4, u1 < u2, n1 < n2)
    return true;
}

bool test_gt(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_GT", 1, u1 > u2, n1 > n2)
    TEST("TEST_GT", 2, u1 > u2, n1 > n2)
    TEST("TEST_GT", 3, u1 > u2, n1 > n2)
    TEST("TEST_GT", 4, u1 > u2, n1 > n2)
    return true;
}

bool test_lte(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_LTE", 1, u1 <= u2, n1 <= n2)
    TEST("TEST_LTE", 2, u2 <= u1, n2 <= n1)
    TEST("TEST_LTE", 3, u1 <= u1, n1 <= n1)
    TEST("TEST_LTE", 4, u2 <= u2, n2 <= n2)
    return true;
}

bool test_gte(uint64_t n1, uint64_t n2) {
    atn::uInt u1 = n1, u2 = n2;
    TEST("TEST_GTE", 1, u1 >= u2, n1 >= n2)
    TEST("TEST_GTE", 2, u2 >= u1, n2 >= n1)
    TEST("TEST_GTE", 3, u1 >= u1, n1 >= n1)
    TEST("TEST_GTE", 4, u2 >= u2, n2 >= n2)
    return true;
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
    for (uint64_t i = 0; i < N; ++i) {
        uint64_t rand1 = rand();
        uint64_t rand2 = rand();
        std::cout << "=======================================" << std::endl;
        std::cout << "RUN #" << (i + 1) << ", n1=" << rand1 << ", n2=" << rand2 << std::endl;
        bool result = test_all(rand1, rand2);
        std::cout << "=======================================" << std::endl << std::endl;
        if (!result) {
            throw std::runtime_error("TEST #" + std::to_string(i + 1) + " HAS FAILED");
        }
    }
    std::cout << "ALL TESTS SUCCEEDED" << std::endl;
}