// Written by Aaron Barge
// Copyright 2020

// The Goal:
// Implement an unsigned Integer class with arbitrary precision. Also this may
// not be considered arbitrary technically. This is because it'll have an upper
// limit depending on the maximum size of a vector. However, as
// std::vector<bool>'s have been optimized to be vectors of bits it should have
// a large maximum limit. This could allow for potentially 100's of thousands
// of bits per uInt.

// New Goal:
// Optimize this implementation until I can calculate the fibonacci sequence at
// 100,000 in under 2 mins with optimization flag -O3

#ifndef _UINTEGER_UINT_HPP_
#define _UINTEGER_UINT_HPP_

#include <climits> // CHAR_BIT
#include <numeric> // std::accumulate
#include <stdexcept> // runtime_error
#include <string> // std::string
#include <vector> // std::vector
#ifdef PERFORMANCE_TEST
    #include <iomanip> // std::setw
    #include <iostream> // std::cout
    #include <chrono> // std::chrono
#endif

namespace atn { // AaronTheNerd

// ====================== Performance Testing Variables =======================

#ifdef PERFORMANCE_TEST
    #define TEST_RESOLUTION std::chrono::duration<double, std::milli>
    #define CLOCK std::chrono::high_resolution_clock
    enum {
        REMOVE_LEAD_ZEROS_TIME,
        INT_TO_UINT_TIME,
        STRING_TO_UINT_TIME,
        COPY_TIME,
        ADD_TIME,
        SUB_TIME,
        MUL_TIME,
        DIV_TIME,
        MOD_TIME,
        SL_TIME,
        SR_TIME,
        AND_TIME,
        OR_TIME,
        XOR_TIME,
        EQ_TIME,
        NEQ_TIME,
        LT_TIME,
        GT_TIME,
        LTE_TIME,
        GTE_TIME,
        UINT_TO_INT_TIME,
        UINT_TO_STRING_TIME,
        NUM_OF_TESTS
    };
    std::vector<std::chrono::time_point<CLOCK>> start_times(NUM_OF_TESTS);
    std::vector<TEST_RESOLUTION> durations(NUM_OF_TESTS);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = stop - stop;
    #define START_TEST(x) \
        start_times[x] = CLOCK::now();
    #define END_TEST(x) \
        stop = CLOCK::now(); \
        duration = stop - start_times[x]; \
        durations[x] += duration;
#endif

// ============================================================================
// ============================== Predefinitions ==============================
// ============================================================================

class uInt {
  private:
    // =========================== Private Methods ============================
    std::pair<uInt, uInt> div_and_mod(const uInt&) const;
    void remove_lead_zeros();
    void convert_binary_string(std::string);
    void convert_hex_string(std::string);
    inline bool odd(char) const;
    std::string div_by_2(std::string) const;
    void convert_decimal_string(std::string);
  public:
    std::vector<bool> bits;
    // ============================= Constructors =============================
    uInt();
    uInt(const uint64_t&);
    explicit uInt(const std::string&);
    uInt(const uInt&);
    // ============================== To String ===============================
    std::string to_string() const;
    std::string to_string(const uint64_t&) const;
    // ============================== Assignment ==============================
    uInt& operator+=(const uInt&);
    uInt& operator-=(const uInt&);
    uInt& operator*=(const uInt&);  
    uInt& operator/=(const uInt&);  
    uInt& operator%=(const uInt&);
    uInt& operator<<=(const uint64_t&);
    uInt& operator>>=(const uint64_t&);
    uInt& operator&=(const uInt&);
    uInt& operator|=(const uInt&);
    uInt& operator^=(const uInt&);
    // ============================== Arithmetic ==============================
    uInt& operator+(const uInt&) const;
    uInt& operator-(const uInt&) const;
    uInt& operator*(const uInt&) const;
    uInt& operator/(const uInt&) const;
    uInt& operator%(const uInt&) const;
    uInt& operator<<(const uint64_t&) const;
    uInt& operator>>(const uint64_t&) const;
    uInt& operator&(const uInt&) const;
    uInt& operator|(const uInt&) const;
    uInt& operator^(const uInt&) const;
    // ================================ Unary =================================
    uInt& operator++();
    uInt& operator--();
    // ============================= Conditional ==============================
    bool operator==(const uInt&) const;
    bool operator!=(const uInt&) const;
    bool operator<(const uInt&) const;
    bool operator>(const uInt&) const;
    bool operator<=(const uInt&) const;
    bool operator>=(const uInt&) const;
    // =============================== Casting ================================
    explicit operator uint64_t() const;
};

// ============================= Helper Variables =============================

static constexpr uint64_t const& negative_one = uint64_t(-1);

const std::string base64_index("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
const std::string base_n_index("0123456789abcdefghijklmnopqrstuvwxyz");

const uInt ZERO = uInt(0);
const uInt ONE = uInt(1);
const uInt TWO = uInt(2);
const uInt THREE = uInt(3);
const uInt FOUR = uInt(4);
const uInt FIVE = uInt(5);
const uInt SIX = uInt(6);
const uInt SEVEN = uInt(7);
const uInt EIGHT = uInt(8);
const uInt NINE = uInt(9);
const uInt TEN = uInt(10);

// ============================================================================
// =============================== Definitions ================================
// ============================================================================

// ============================= Private Methods ==============================

// Performs a simultaneous division and modulo operation and returns a pair
// containing the results where the first item in the pair is an atn::uInt
// representing the result of the division and the second is an atn::uInt
// representing the result of the modulo.
std::pair<uInt, uInt> uInt::div_and_mod(const uInt& n) const {
    /*
    Potential micro optimizations
    1. Iterator vs. **uint64_t index**
    2. Setting the quotient's value to `mod >= n` vs. **to true or false.**
    3. Adding quotient[i] / *it's value to mod directly vs. **if statement**
    */
    if (n == ZERO) {
        throw std::runtime_error("ERROR: Divide/Mod by 0 Exception");
    }
    if (this->bits.size() == 0) {
        return std::make_pair(*this, *this);
    }
    uInt quotient(*this), mod;
    for (uint64_t i = quotient.bits.size() - 1; i != negative_one; --i) {
        mod <<= 1;
        if (quotient.bits[i]) ++mod;
        if (mod >= n) {
            quotient.bits[i] = true;
            mod -= n;
        } else {
            quotient.bits[i] = false;
        }
    }
    mod.remove_lead_zeros();
    quotient.remove_lead_zeros();
    return std::make_pair(quotient, mod);
}

void uInt::remove_lead_zeros() {
    #ifdef PERFORMANCE_TEST
        START_TEST(REMOVE_LEAD_ZEROS_TIME)
    #endif
    while (this->bits.size() != 0 && !this->bits.back())
        this->bits.pop_back();
    #ifdef PERFORMANCE_TEST
        END_TEST(REMOVE_LEAD_ZEROS_TIME)
    #endif
}

void uInt::convert_binary_string(std::string str) {
    for (auto it = str.rbegin(); it != str.rend(); ++it) {
        if (*it != '1' && *it != '0') {
            this->bits.clear();
            throw std::runtime_error("ERROR: Cannot input non-binary digits");
        }
        this->bits.emplace_back(*it == '1');
    }
}

void uInt::convert_hex_string(std::string str) {
    for (auto it = str.rbegin(); it != str.rend(); ++it) {
        switch (*it) {
            case 'f': case 'F':
                this->convert_binary_string(std::string("1111"));
                break;
            case 'e': case 'E':
                this->convert_binary_string(std::string("1110"));
                break;
            case 'd': case 'D':
                this->convert_binary_string(std::string("1101"));
                break;
            case 'c': case 'C':
                this->convert_binary_string(std::string("1100"));
                break;
            case 'b': case 'B':
                this->convert_binary_string(std::string("1011"));
                break;
            case 'a': case 'A':
                this->convert_binary_string(std::string("1010"));
                break;
            case '9':
                this->convert_binary_string(std::string("1001"));
                break;
            case '8':
                this->convert_binary_string(std::string("1000"));
                break;
            case '7':
                this->convert_binary_string(std::string("0111"));
                break;
            case '6':
                this->convert_binary_string(std::string("0110"));
                break;
            case '5':
                this->convert_binary_string(std::string("0101"));
                break;
            case '4':
                this->convert_binary_string(std::string("0100"));
                break;
            case '3':
                this->convert_binary_string(std::string("0011"));
                break;
            case '2':
                this->convert_binary_string(std::string("0010"));
                break;
            case '1':
                this->convert_binary_string(std::string("0001"));
                break;
            case '0':
                this->convert_binary_string(std::string("0000"));
                break;
            default:
                this->bits.clear();
                return;
        }
    }
}

bool uInt::odd(char c) const {
    return c == '1' || c == '3' || c == '5' || c == '7' || c == '9';
}

std::string uInt::div_by_2(std::string str) const {
    std::string result;
    uint8_t add = 0, new_digit;
    for (auto it = str.begin(); it != str.end(); ++it) {
        if ('0' > *it || *it > '9') {
            throw std::runtime_error("ERROR: Cannot input non-decimal digits");
        }
        new_digit = (int(*it) - int('0')) / 2 + add;
        result = result + char(new_digit + int('0'));
        add = 5 * odd(*it);
    }
    if (result != std::string("0") && result[0] == '0') {
        result = result.substr(1, result.size() - 1);
    }
    return result;
}

// https://stackoverflow.com/questions/11006844/convert-a-very-large-number-from-decimal-string-to-binary-representation
void uInt::convert_decimal_string(std::string str) {
    while (str != std::string("0") && str[0] == '0') {
        str = str.substr(1, str.size() - 1);
    }
    if (str == std::string("0")) {
        this->bits.clear();
        return;
    }
    while (str != std::string("0")) {
        this->bits.emplace_back(odd(*(str.rbegin())));
        str = div_by_2(str);
    }
}

// ============================== Public Methods ==============================

// =============================== Constructors ===============================

uInt::uInt() : bits(std::vector<bool>()) {}

uInt::uInt(const uint64_t& num)
        : bits(std::vector<bool>()) {
    #ifdef PERFORMANCE_TEST
        START_TEST(INT_TO_UINT_TIME)
    #endif
    uint64_t copy = num;
    while (copy != 0) {
        this->bits.emplace_back(copy & 1 ? true : false);
        copy >>= 1;
    }
    this->remove_lead_zeros();
    #ifdef PERFORMANCE_TEST
        END_TEST(INT_TO_UINT_TIME)
    #endif
}

uInt::uInt(const std::string& str)
        : bits(std::vector<bool>()) {
    #ifdef PERFORMANCE_TEST
        START_TEST(STRING_TO_UINT_TIME)
    #endif
    if (str.size() == 0) {
        this->bits.clear();
        return;
    }
    if (str.size() < 3) { // Decimal
        this->convert_decimal_string(str);
        this->remove_lead_zeros();
        return;
    }
    std::string base = str.substr(0, 2);
    if (base == std::string("0x") || base == std::string("0X")) {
        this->convert_hex_string(str.substr(2, str.size() - 2));
    } else if (base == std::string("0b") || base == std::string("0B")) {
        this->convert_binary_string(str.substr(2, str.size() - 2));
    } else {
        this->convert_decimal_string(str);
    }
    this->remove_lead_zeros();
    #ifdef PERFORMANCE_TEST
        END_TEST(STRING_TO_UINT_TIME)
    #endif
}

uInt::uInt(const uInt& n)
        : bits(std::vector<bool>()) {
    #ifdef PERFORMANCE_TEST
        START_TEST(COPY_TIME)
    #endif
    if (this != &n) {
        this->bits.resize(n.bits.size(), false);
        for (uint64_t i = 0; i < this->bits.size(); ++i)
            this->bits[i] = n.bits[i];
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(COPY_TIME)
    #endif
}

// ================================ To String =================================

std::string uInt::to_string() const {
    #ifdef PERFORMANCE_TEST
        START_TEST(UINT_TO_STRING_TIME)
    #endif
    if (this->bits.empty()) return std::string("0");
    uInt n(*this), mod;
    std::string result("");
    while (n.bits.size()) {
        std::pair<uInt, uInt> div_mod_result = n.div_and_mod(TEN);
        mod = div_mod_result.second;
        n = div_mod_result.first;
        if (mod.bits.empty())
            result = '0' + result;
        else if (mod.bits.size() == 1)
            result = '1' + result;
        else if (mod == TWO)
            result = '2' + result;
        else if (mod == THREE)
            result = '3' + result;
        else if (mod == FOUR)
            result = '4' + result;
        else if (mod == FIVE)
            result = '5' + result;
        else if (mod == SIX)
            result = '6' + result;
        else if (mod == SEVEN)
            result = '7' + result;
        else if (mod == EIGHT)
            result = '8' + result;
        else if (mod == NINE)
            result = '9' + result;
        else {
            return "";
        }
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(UINT_TO_STRING_TIME)
    #endif
    return result;
}

std::string uInt::to_string(const uint64_t& base) const {
    std::string result_str("");
    if (base == 2u) {
        for (auto it = this->bits.rbegin(); it != this->bits.rend(); ++it) {
            result_str.append(1, *it ? '1' : '0');
        }
        return result_str;
    }
    if (base == 10u) {
        return this->to_string();
    }
    if (base == 64u) {
        if (*this == ZERO) return std::string("0");
        uInt uint_base(base), copy(*this), mod;
        while (copy != ZERO) {
            std::pair<uInt, uInt> div_mod_result = copy.div_and_mod(uint_base);
            mod = div_mod_result.second;
            copy = div_mod_result.first;
            result_str = base64_index[uint64_t(mod)] + result_str;
        }
        return result_str;
    }
    if (base <= 36u) {
        if (*this == ZERO) return std::string("0");
        uInt uint_base(base), copy(*this), mod;
        while (copy != ZERO) {
            std::pair<uInt, uInt> div_mod_result = copy.div_and_mod(uint_base);
            mod = div_mod_result.second;
            copy = div_mod_result.first;
            result_str = base_n_index[uint64_t(mod)] + result_str;
        }
        return result_str;
    }
    throw std::runtime_error("ERROR: Unaccepted base: " + base);
}

// ================================ Assignment ================================

// ============================== Add and Assign ==============================
uInt& uInt::operator+=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
            START_TEST(ADD_TIME)
        #endif
        bool n1 = 0, n2 = 0, carry = 0;
        if (this->bits.size() < n.bits.size()) {
            this->bits.resize(n.bits.size(), false);
        }
        uint64_t i;
        for (i = 0; i < n.bits.size(); ++i) {
            n1 = this->bits[i];
            n2 = n.bits[i];
            this->bits[i] = carry != (n1 != n.bits[i]);
            carry = (n1 && n2) || (n1 && carry) || (n2 && carry);
        }
        for (; i < this->bits.size(); ++i) {
            n1 = this->bits[i];
            this->bits[i] = carry != n1;
            carry = n1 && carry;
        }
        if (carry)
            this->bits.emplace_back(true);
        #ifdef PERFORMANCE_TEST
            END_TEST(ADD_TIME)
        #endif
        return *this; 
}

// ============================= Minus and Assign =============================
uInt& uInt::operator-=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
        START_TEST(SUB_TIME)
    #endif
    if (*this <= n) {
        this->bits.clear();
        #ifdef PERFORMANCE_TEST
            START_TEST(SUB_TIME)
        #endif
        return *this;
    }
    bool n1 = 0, n2 = 0;
    uint64_t this_end = this->bits.size(), n_end = n.bits.size(), j;
    for (uint64_t i = 0; i < n_end; ++i) {
        n1 = this->bits[i];
        n2 = n.bits[i];
        this->bits[i] = n1 != n2;
        if (n2 && !n1) {
            for (j = i + 1; j < this_end; ++j) {
                this->bits[j] = !this->bits[j];
                if (!this->bits[j]) {
                    break;
                }
            }
            if (j == this_end) {
                this->bits.clear();
                #ifdef PERFORMANCE_TEST
                    END_TEST(SUB_TIME)
                #endif
                return *this;
            }
        }
    }
    this->remove_lead_zeros();
    #ifdef PERFORMANCE_TEST
        END_TEST(SUB_TIME)
    #endif
    return *this;
}

// =========================== Multiply and Assign ============================
uInt& uInt::operator*=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
        START_TEST(MUL_TIME)
    #endif
    uInt mult, shifted_n(n);
    uint64_t end = this->bits.size();
    for (uint64_t i = 0; i < end; ++i) {
        if (this->bits[i]) mult += shifted_n;
        shifted_n <<= 1;
    }
    this->bits = mult.bits;
    #ifdef PERFORMANCE_TEST
        END_TEST(MUL_TIME)
    #endif
    return *this;
}

// ============================ Divide and Assign =============================
uInt& uInt::operator/=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
        START_TEST(DIV_TIME)
    #endif
    if (n == 0) {
        throw std::runtime_error("ERROR: Divide by 0 Exception");
    }
    if (this->bits.size() == 0) {
        #ifdef PERFORMANCE_TEST
            START_TEST(DIV_TIME)
        #endif
        return *this;
    }
    uInt curr(0);
    for (uint64_t i = this->bits.size() - 1; i != negative_one; --i) {
        curr <<= 1;
        if (this->bits[i]) ++curr;
        if (curr >= n) {
            this->bits[i] = true;
            curr -= n;
        } else {
            this->bits[i] = false;
        }
    }
    this->remove_lead_zeros();
    #ifdef PERFORMANCE_TEST
        END_TEST(DIV_TIME)
    #endif
    return *this;
}

// ============================== Mod and Assign ==============================
uInt& uInt::operator%=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
        START_TEST(MOD_TIME)
    #endif
    uInt curr(0);
    for (uint64_t i = this->bits.size() - 1; i != negative_one; --i) {
        curr <<= 1;
        if (this->bits[i]) ++curr;
        if (curr >= n) {
            curr -= n;
        }
    }
    this->bits = curr.bits;
    this->remove_lead_zeros();
    #ifdef PERFORMANCE_TEST
        END_TEST(MOD_TIME)
    #endif
    return *this;
}

// ========================== Shift Left and Assign ===========================
uInt& uInt::operator<<=(const uint64_t& pos) {
    #ifdef PERFORMANCE_TEST
        START_TEST(SL_TIME)
    #endif
    if (this->bits.size() == 0 || pos == 0) return *this;
    this->bits.insert(this->bits.begin(), pos, false);
    #ifdef PERFORMANCE_TEST
        END_TEST(SL_TIME)
    #endif
    return *this;
}

// ========================== Shift Right and Assign ==========================
uInt& uInt::operator>>=(const uint64_t& pos) {
    #ifdef PERFORMANCE_TEST
        START_TEST(SR_TIME)
    #endif
    if (this->bits.size() == 0 || pos == 0) return *this;
    this->bits.erase(this->bits.begin(), this->bits.begin() + pos);
    #ifdef PERFORMANCE_TEST
        END_TEST(SR_TIME)
    #endif
    return *this;
}

// ========================== Bitwise AND and Assign ==========================
uInt& uInt::operator&=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
        START_TEST(AND_TIME)
    #endif
    if (this->bits.size() > n.bits.size())
        this->bits.resize(n.bits.size(), false);
    uint64_t end = this->bits.size();
    for (uint64_t i = 0; i < end; ++i) {
        this->bits[i] = this->bits[i] && n.bits[i];
    }
    this->remove_lead_zeros();
    #ifdef PERFORMANCE_TEST
        END_TEST(AND_TIME)
    #endif
    return *this;
}

// ========================== Bitwise OR and Assign ===========================
uInt& uInt::operator|=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
        START_TEST(OR_TIME)
    #endif
    if (this->bits.size() < n.bits.size())
        this->bits.resize(n.bits.size(), false);
    uint64_t end = n.bits.size();
    for (uint64_t i = 0; i < end; ++i) {
        this->bits[i] = this->bits[i] || n.bits[i];
    }
    this->remove_lead_zeros();
    #ifdef PERFORMANCE_TEST
        END_TEST(OR_TIME)
    #endif
    return *this;
}

// ========================== Bitwise XOR and Assign ==========================
uInt& uInt::operator^=(const uInt& n) {
    #ifdef PERFORMANCE_TEST
            START_TEST(XOR_TIME)
        #endif
        if (this->bits.size() < n.bits.size())
            this->bits.resize(n.bits.size(), false);
        uint64_t end = n.bits.size();
        for (uint64_t i = 0; i < end; ++i) {
            this->bits[i] = this->bits[i] ^ n.bits[i];
        }
        this->remove_lead_zeros();
        #ifdef PERFORMANCE_TEST
            END_TEST(XOR_TIME)
        #endif
        return *this;
}

// ================================ Arithmetic ================================

uInt& uInt::operator+(const uInt& n) const {
    uInt* sum = new uInt(*this);
    *sum += n;
    return *sum;
}

uInt& uInt::operator-(const uInt& n) const {
    uInt* difference = new uInt(*this);
    *difference -= n;
    return *difference;
}

uInt& uInt::operator*(const uInt& n) const {
    uInt* product = new uInt(*this);
    *product *= n;
    return *product;
}

uInt& uInt::operator/(const uInt& n) const {
    uInt* quotient = new uInt(*this);
    *quotient /= n;
    return *quotient;
}

uInt& uInt::operator%(const uInt& n) const {
    uInt* mod = new uInt(*this);
    *mod %= n;
    return *mod;
}

uInt& uInt::operator<<(const uint64_t& pos) const {
    uInt* lsh = new uInt(*this);
    *lsh <<= pos;
    return *lsh;
}

uInt& uInt::operator>>(const uint64_t& pos) const {
    uInt* rsh = new uInt(*this);
    *rsh >>= pos;
    return *rsh;
}

uInt& uInt::operator&(const uInt& n) const {
    uInt* and_result = new uInt(*this);
    *and_result &= n;
    return *and_result;
}

uInt& uInt::operator|(const uInt& n) const {
    uInt* or_result = new uInt(*this);
    *or_result |= n;
    return *or_result;
}

uInt& uInt::operator^(const uInt& n) const {
    uInt* xor_result = new uInt(*this);
    *xor_result ^= n;
    return *xor_result;
}

// ================================ Unary =================================

uInt& uInt::operator++() {
    return *this += ONE;
}

uInt& uInt::operator--() {
    return *this -= ONE;
}

// ============================= Conditional ==============================

bool uInt::operator==(const uInt& n) const {
    #ifdef PERFORMANCE_TEST
        START_TEST(EQ_TIME)
    #endif
    if (this->bits.size() != n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(EQ_TIME)
        #endif
        return false;
    }
    uint64_t end = this->bits.size();
    for (uint64_t i = 0; i < end; ++i) {
        if (this->bits[i] != n.bits[i]) {
            #ifdef PERFORMANCE_TEST
                END_TEST(EQ_TIME)
            #endif
            return false;
        }
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(EQ_TIME)
    #endif
    return true;
}

bool uInt::operator!=(const uInt& n) const {
    #ifdef PERFORMANCE_TEST
        START_TEST(NEQ_TIME)
    #endif
    if (this->bits.size() != n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(NEQ_TIME)
        #endif
        return true;
    }
    uint64_t end = this->bits.size();
    for (uint64_t i = 0; i < end; ++i) {
        if (this->bits[i] != n.bits[i]) {
            #ifdef PERFORMANCE_TEST
                END_TEST(NEQ_TIME)
            #endif
            return true;
        }
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(NEQ_TIME)
    #endif
    return false;
}

bool uInt::operator<(const uInt& n) const {
    #ifdef PERFORMANCE_TEST
        START_TEST(LT_TIME)
    #endif
    if (this->bits.size() < n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(LT_TIME)
        #endif
        return true;
    }
    if (this->bits.size() > n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(LT_TIME)
        #endif
        return false;
    }
    for (uint64_t i = this->bits.size() - 1; i != negative_one; --i) {
        if (this->bits[i] != n.bits[i]) {
            #ifdef PERFORMANCE_TEST
                END_TEST(LT_TIME)
            #endif
            return n.bits[i];
        }
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(LT_TIME)
    #endif
    return false;
}

bool uInt::operator>(const uInt& n) const {
    #ifdef PERFORMANCE_TEST
        START_TEST(GT_TIME)
    #endif
    if (this->bits.size() > n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(GT_TIME)
        #endif
        return true;
    }
    if (this->bits.size() < n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(GT_TIME)
        #endif
        return false;
    }
    for (uint64_t i = this->bits.size() - 1; i != negative_one; --i) {
        if (this->bits[i] != n.bits[i]) {
            #ifdef PERFORMANCE_TEST
                END_TEST(GT_TIME)
            #endif
            return this->bits[i];
        }
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(GT_TIME)
    #endif
    return false;
}

bool uInt::operator<=(const uInt& n) const {
    #ifdef PERFORMANCE_TEST
        START_TEST(LTE_TIME)
    #endif
    if (this->bits.size() < n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(LTE_TIME)
        #endif
        return true;
    }
    if (this->bits.size() > n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(LTE_TIME)
        #endif
        return false;
    }
    for (uint64_t i = this->bits.size() - 1; i != negative_one; --i) {
        if (this->bits[i] != n.bits[i]) {
            #ifdef PERFORMANCE_TEST
                END_TEST(LTE_TIME)
            #endif
            return n.bits[i];
        }
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(LTE_TIME)
    #endif
    return true;
}

bool uInt::operator>=(const uInt& n) const {
    #ifdef PERFORMANCE_TEST
        START_TEST(GTE_TIME)
    #endif
    if (this->bits.size() > n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(GTE_TIME)
        #endif
        return true;
    }
    if (this->bits.size() < n.bits.size()) {
        #ifdef PERFORMANCE_TEST
            END_TEST(GTE_TIME)
        #endif
        return false;
    }
    for (uint64_t i = this->bits.size() - 1; i != negative_one; --i) {
        if (this->bits[i] != n.bits[i]) {
            #ifdef PERFORMANCE_TEST
                END_TEST(GTE_TIME)
            #endif
            return this->bits[i];
        }
    }
    #ifdef PERFORMANCE_TEST
        END_TEST(GTE_TIME)
    #endif
    return true;
}

// =============================== Casting ================================

uInt::operator uint64_t() const {
    #ifdef PERFORMANCE_TEST
        START_TEST(UINT_TO_INT_TIME)
    #endif
    uint64_t result = std::accumulate(this->bits.rbegin(), this->bits.rend(), 0ull, [](uint64_t x, bool y) { return (x << 1) + y; });
    #ifdef PERFORMANCE_TEST
        END_TEST(UINT_TO_INT_TIME)
    #endif
    return result;
}

// =========================== Non-Class Functions ============================

std::ostream& operator<<(std::ostream& out, const uInt& n) {
    out << n.to_string();
    return out;
}

uInt& operator+(const uint64_t& num, const uInt& n) {
    return n + num;
}

uInt& operator-(const uint64_t& num, const uInt& n) {
    return uInt(num) - n;
}

uInt& operator*(const uint64_t& num, const uInt& n) {
    return n * num;
}

uInt& operator/(const uint64_t& num, const uInt& n) {
    return uInt(num) / n;
}

uInt& operator%(const uint64_t& num, const uInt& n) {
    return uInt(num) % n;
}

uInt& operator&(const uint64_t& num, const uInt& n) {
    return n & num;
}

uInt& operator|(const uint64_t& num, const uInt& n) {
    return n | num;
}

uInt& operator^(const uint64_t& num, const uInt& n) {
    return n ^ num;
}
// =========================== Performance Testing ============================

#ifdef PERFORMANCE_TEST
void print_performance_test_results() {
    double total = 0.0;
    for (auto time : durations) {
        total += time.count();
    }
    std::cout << std::endl << "PERFORMANCE TEST RESULTS" << std::endl;
    std::cout << "======================================================" << std::endl;
    for (uint64_t i = 0; i < NUM_OF_TESTS; ++i) {
        double time = durations[i].count();
        std::cout << std::setw(27);
        switch (i) {
            case REMOVE_LEAD_ZEROS_TIME:
                std::cout << "REMOVE_LEAD_ZEROS";
                break;
            case INT_TO_UINT_TIME:
                std::cout << "INT TO UINT";
                break;
            case STRING_TO_UINT_TIME:
                std::cout << "STRING TO UINT";
                break;
            case COPY_TIME:
                std::cout << "COPY";
                break;
            case ADD_TIME:
                std::cout << "ADDITION";
                break;
            case SUB_TIME:
                std::cout << "SUBTRACTION";
                break;
            case MUL_TIME:
                std::cout << "MULTIPLICATION";
                break;
            case DIV_TIME:
                std::cout << "DIVISION";
                break;
            case MOD_TIME:
                std::cout << "MODULO";
                break;
            case SL_TIME:
                std::cout << "SHIFT LEFT";
                break;
            case SR_TIME:
                std::cout << "SHIFT RIGHT";
                break;
            case AND_TIME:
                std::cout << "BITWISE AND";
                break;
            case OR_TIME:
                std::cout << "BITWISE OR";
                break;
            case XOR_TIME:
                std::cout << "BITWISE XOR";
                break;
            case EQ_TIME:
                std::cout << "EQUALS";
                break;
            case NEQ_TIME:
                std::cout << "NOT EQUALS";
                break;
            case LT_TIME:
                std::cout << "LESS THAN";
                break;
            case GT_TIME:
                std::cout << "GREATER THAN";
                break;
            case LTE_TIME:
                std::cout << "GREATER THAN OR EQUAL TO";
                break;
            case GTE_TIME:
                std::cout << "LESS THAN OR EQUAL TO";
                break;
            case UINT_TO_INT_TIME:
                std::cout << "UINT TO INT";
                break;
            case UINT_TO_STRING_TIME:
                std::cout << "UINT TO STRING";
                break;
            default:
                std::cout << "";
        }
        std::cout << " || " << std::setw(8) << time << "ms, " << (time / total * 100) << "%" << std::endl;
    }
}
#endif

} // End namespace atn

#endif // _UINTEGER_UINT_HPP_