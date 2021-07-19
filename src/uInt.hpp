// Written by Aaron Barge
// Copyright 2020


// The Goal: NEEDS TO BE TESTED
// Implement an unsigned Integer class with arbitrary precision. Also this may
// not be considered arbitrary technically. This is because it'll have an upper
// limit depending on the maximum size of a vector. However, as
// std::vector<bool>'s have been optimized to be vectors of bits it should have
// a large maximum limit. This could allow for potentially 100's of thousands
// of bits per uInt.

// New Goal:
// Optimize this implementation until I can calculate the fibonacci sequence at
// 100,000 in under 1 min with optimization flag -O3


#ifndef _UINTEGER_UINT_HPP_
#define _UINTEGER_UINT_HPP_


#define PERFORMANCE_TEST 1


#include <climits> // CHAR_BIT
#include <stdexcept> // runtime_error
#include <string> // std::string
#include <vector> // std::vector


#if PERFORMANCE_TEST
    #include <algorithm>
    #include <iomanip>
    #include <iostream>
    #include <chrono>
#endif


namespace atn { // AaronTheNerd


#if PERFORMANCE_TEST
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


// =========================== Class Declaration =========================== //
class uInt {


  private:
    // ========================== Helper Methods =========================== //
    std::pair<std::vector<bool>, std::vector<bool>> div_and_mod(const uInt& n) const {
        if (n == 0) {
            throw std::runtime_error("ERROR: Divide/Mod by 0 Exception");
        }
        if (this->bits.size() == 0) {
            return std::make_pair(this->bits, this->bits);
        }
        uInt quotient(*this), mod(0);
        for (auto it = quotient.bits.rbegin(); it != quotient.bits.rend(); it++) {
            mod <<= 1;
            if (*it) ++mod;
            if (mod >= n) {
                *it = true;
                mod -= n;
            } else {
                *it = false;
            }
        }
        mod.remove_lead_zeros();
        quotient.remove_lead_zeros();
        return std::make_pair(quotient.bits, mod.bits);
    }
    void remove_lead_zeros() {
        #if PERFORMANCE_TEST
            START_TEST(REMOVE_LEAD_ZEROS_TIME)
        #endif
        while (!this->bits.back() && this->bits.size() != 0)
            this->bits.pop_back();
        #if PERFORMANCE_TEST
            END_TEST(REMOVE_LEAD_ZEROS_TIME)
        #endif
    }


    // ========================= String Conversions ======================== //
    // ====================== Binary String Conversion ===================== //
    void convert_binary_string(std::string str) {
        for (auto it = str.rbegin(); it != str.rend(); ++it) {
            if (*it != '1' && *it != '0') {
                this->bits.clear();
                return;
            }
            this->bits.emplace_back((*it == '1') ? true : false);
        }
    }


    // =================== Hexidecimal String Conversion =================== //
    void convert_hex_string(std::string str) {
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
    // ===================== Decimal String Conversion ===================== //
    // https://stackoverflow.com/questions/11006844/convert-a-very-large-number-from-decimal-string-to-binary-representation
    bool odd(char c) const {
        return c == '1' || c == '3' || c == '5' || c == '7' || c == '9';
    }
    std::string div_by_2(std::string str) const {
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
    void convert_decimal_string(std::string str) {
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


  public:
    std::vector<bool> bits;


    // =========================== Constructors ============================ //
    uInt() : bits(std::vector<bool>()) {}
    uInt(const uint64_t& num) 
            : bits(std::vector<bool>()) {
        #if PERFORMANCE_TEST
            START_TEST(INT_TO_UINT_TIME)
        #endif
        uint64_t ull_size = sizeof(uint64_t) * CHAR_BIT;
        for (uint64_t i = 0; i < ull_size; ++i) {
            this->bits.emplace_back(((num & (1ull << i)) >> i) ? true : false);
        }
        this->remove_lead_zeros();
        #if PERFORMANCE_TEST
            END_TEST(INT_TO_UINT_TIME)
        #endif
    }
    explicit uInt(const std::string& str) 
            : bits(std::vector<bool>()) {
        #if PERFORMANCE_TEST
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
        #if PERFORMANCE_TEST
            END_TEST(STRING_TO_UINT_TIME)
        #endif
    }
    uInt(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(COPY_TIME)
        #endif
        if (this != &n) {
            this->bits.resize(n.bits.size(), false);
            for (int i = 0; i < this->bits.size(); ++i)
                this->bits[i] = n.bits[i];
        }
        #if PERFORMANCE_TEST
            END_TEST(COPY_TIME)
        #endif
    }


    // ============================= To String ============================= //
    std::string to_string() const;
    std::string to_string(const uint64_t&) const;
    

    // ========================== Add and Assign =========================== //
    uInt& operator+=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(ADD_TIME)
        #endif
        bool n1 = 0, n2 = 0, carry = 0;
        if (this->bits.size() < n.bits.size()) {
            this->bits.resize(n.bits.size(), false);
        }
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            n1 = this->bits[i];
            n2 = (i >= n.bits.size()) ? false : n.bits[i];
            this->bits[i] = carry ^ (n1 ^ n2);
            carry = (n1 && n2) || (n1 && carry) || (n2 && carry);
        }
        if (carry)
            this->bits.emplace_back(true);
        #if PERFORMANCE_TEST
            END_TEST(ADD_TIME)
        #endif
        return *this;    
    }
    
    
    // ========================= Minus and Assign ========================== //
    uInt& operator-=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(SUB_TIME)
        #endif
        if (*this <= n) {
            this->bits.clear();
            return *this;
        }
        bool n1 = 0, n2 = 0;
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            n1 = this->bits[i];
            n2 = (i >= n.bits.size()) ? false : n.bits[i];
            this->bits[i] = n1 ^ n2;
            if (n2 && !n1) {
                for (uint64_t j = i + 1; j < this->bits.size(); ++j) {
                    if (this->bits[j]) {
                        this->bits[j] = false;
                        break;
                    } else {
                        this->bits[j] = true;
                    }
                }
            }
        }
        this->remove_lead_zeros();
        #if PERFORMANCE_TEST
            END_TEST(SUB_TIME)
        #endif
        return *this;
    }
    
    
    // ======================== Multiply and Assign ======================== //
    uInt& operator*=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(MUL_TIME)
        #endif
        uInt mult(0), shifted_n(n);
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            if (this->bits[i]) mult += shifted_n;
            shifted_n <<= 1;
        }
        this->bits = mult.bits;
        #if PERFORMANCE_TEST
            END_TEST(MUL_TIME)
        #endif
        return *this;
    }
    
    
    // ========================= Divide and Assign ========================= //    
    uInt& operator/=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(DIV_TIME)
        #endif
        if (n == 0) {
            throw std::runtime_error("ERROR: Divide by 0 Exception");
        }
        if (this->bits.size() == 0) {
            return *this;
        }
        uInt curr(0);
        for (auto it = this->bits.rbegin(); it != this->bits.rend(); it++) {
            curr <<= 1;
            if (*it) ++curr;
            if (curr >= n) {
                *it = true;
                curr -= n;
            } else {
                *it = false;
            }
        }
        this->remove_lead_zeros();
        #if PERFORMANCE_TEST
            END_TEST(DIV_TIME)
        #endif
        return *this;
    }
    
    
    // ========================== Mod and Assign =========================== //    
    uInt& operator%=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(MOD_TIME)
        #endif
        uInt curr(0);
        for (auto it = this->bits.rbegin(); it != this->bits.rend(); it++) {
            curr <<= 1;
            if (*it) ++curr;
            if (curr >= n) {
                *it = true;
                curr -= n;
            } else {
                *it = false;
            }
        }
        this->bits = curr.bits;
        this->remove_lead_zeros();
        #if PERFORMANCE_TEST
            END_TEST(MOD_TIME)
        #endif
        return *this;
    }


    // ======================= Left Shift And Assign ======================= //
    uInt& operator<<=(const uint64_t& pos) {
        #if PERFORMANCE_TEST
            START_TEST(SL_TIME)
        #endif
        if (this->bits.size() == 0 || pos == 0) return *this;
        this->bits.insert(this->bits.begin(), pos, false);
        #if PERFORMANCE_TEST
            END_TEST(SL_TIME)
        #endif
        return *this;
    }


    // ======================= Right Shift And Assign ====================== //
    uInt& operator>>=(const uint64_t& pos) {
        #if PERFORMANCE_TEST
            START_TEST(SR_TIME)
        #endif
        if (this->bits.size() == 0 || pos == 0) return *this;
        this->bits.erase(this->bits.begin(), this->bits.begin() + pos);
        #if PERFORMANCE_TEST
            END_TEST(SR_TIME)
        #endif
        return *this;
    }


    // ====================== Bitwise AND And Assign ======================= //
    uInt& operator&=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(AND_TIME)
        #endif
        if (this->bits.size() > n.bits.size())
            this->bits.resize(n.bits.size(), false);
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            this->bits[i] = this->bits[i] && n.bits[i];
        }
        this->remove_lead_zeros();
        #if PERFORMANCE_TEST
            END_TEST(AND_TIME)
        #endif
        return *this;
    }


    // ======================= Bitwise OR And Assign ======================= //
    uInt& operator|=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(OR_TIME)
        #endif
        if (this->bits.size() < n.bits.size())
            this->bits.resize(n.bits.size(), false);
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            this->bits[i] = this->bits[i] || n.bits[i];
        }
        this->remove_lead_zeros();
        #if PERFORMANCE_TEST
            END_TEST(OR_TIME)
        #endif
        return *this;
    }


    // ====================== Bitwise XOR And Assign ======================= //
    uInt& operator^=(const uInt& n) {
        #if PERFORMANCE_TEST
            START_TEST(XOR_TIME)
        #endif
        if (this->bits.size() < n.bits.size())
            this->bits.resize(n.bits.size(), false);
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            this->bits[i] = this->bits[i] ^ n.bits[i];
        }
        this->remove_lead_zeros();
        #if PERFORMANCE_TEST
            END_TEST(XOR_TIME)
        #endif
        return *this;
    }

    // ============================= Arithmetic ============================ //
    uInt& operator+(const uInt& n) const {
        uInt* sum = new uInt(*this);
        *sum += n;
        return *sum;
    }
    
    
    uInt& operator-(const uInt& n) const {
        uInt* difference = new uInt(*this);
        *difference -= n;
        return *difference;
    }
    
    
    uInt& operator*(const uInt& n) const {
        uInt* product = new uInt(*this);
        *product *= n;
        return *product;
    }
    
    
    uInt& operator/(const uInt& n) const {
        uInt* quotient = new uInt(*this);
        *quotient /= n;
        return *quotient;
    }
    
    
    uInt& operator%(const uInt& n) const {
        uInt* mod = new uInt(*this);
        *mod %= n;
        return *mod;
    }
    
    
    uInt& operator<<(const uint64_t& pos) const {
        uInt* lsh = new uInt(*this);
        *lsh <<= pos;
        return *lsh;
    }
    
    
    uInt& operator>>(const uint64_t& pos) const {
        uInt* rsh = new uInt(*this);
        *rsh >>= pos;
        return *rsh;
    }


    uInt& operator&(const uInt& n) const {
        uInt* and_result = new uInt(*this);
        *and_result &= n;
        return *and_result;
    }


    uInt& operator|(const uInt& n) const {
        uInt* or_result = new uInt(*this);
        *or_result |= n;
        return *or_result;
    }


    uInt& operator^(const uInt& n) const {
        uInt* xor_result = new uInt(*this);
        *xor_result ^= n;
        return *xor_result;
    }
    
    
    // ========================== Unary Operators ========================== //
    uInt& operator++();


    uInt& operator--();
    

    // =========================== Conditionals ============================ //
    bool operator==(const uInt& n) const {
        #if PERFORMANCE_TEST
            START_TEST(EQ_TIME)
        #endif
        if (this->bits.size() != n.bits.size())
            return false;
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            if (this->bits[i] ^ n.bits[i])
                return false;
        }
        #if PERFORMANCE_TEST
            END_TEST(EQ_TIME)
        #endif
        return true;
    }
    
    
    bool operator!=(const uInt& n) const {
        #if PERFORMANCE_TEST
            START_TEST(NEQ_TIME)
        #endif
        if (this->bits.size() != n.bits.size())
            return true;
        for (uint64_t i = 0; i < this->bits.size(); ++i) {
            if (this->bits[i] ^ n.bits[i])
                return true;
        }
        #if PERFORMANCE_TEST
            END_TEST(NEQ_TIME)
        #endif
        return false;
    }
    
    
    bool operator<(const uInt& n) const {
        #if PERFORMANCE_TEST
            START_TEST(LT_TIME)
        #endif
        if (this->bits.size() < n.bits.size())
            return true;
        else if (this->bits.size() > n.bits.size())
            return false;
        for (uint64_t i = this->bits.size(); i != uint64_t(-1); --i) {
            if (this->bits[i] ^ n.bits[i])
                return n.bits[i];
        }
        #if PERFORMANCE_TEST
            END_TEST(LT_TIME)
        #endif
        return false;
    }
    
    
    bool operator>(const uInt& n) const {
        #if PERFORMANCE_TEST
            START_TEST(GT_TIME)
        #endif
        if (this->bits.size() > n.bits.size())
            return true;
        else if (this->bits.size() < n.bits.size())
            return false;
        for (uint64_t i = this->bits.size(); i != uint64_t(-1); --i) {
            if (this->bits[i] ^ n.bits[i])
                return this->bits[i];
        }
        #if PERFORMANCE_TEST
            END_TEST(GT_TIME)
        #endif
        return false;
    }
    
    
    bool operator<=(const uInt& n) const {
        #if PERFORMANCE_TEST
            START_TEST(LTE_TIME)
        #endif
        if (this->bits.size() < n.bits.size())
            return true;
        else if (this->bits.size() > n.bits.size())
            return false;
        for (uint64_t i = this->bits.size(); i != uint64_t(-1); --i) {
            if (this->bits[i] ^ n.bits[i])
                return n.bits[i];
        }
        #if PERFORMANCE_TEST
            END_TEST(LTE_TIME)
        #endif
        return true;
    }
    
    
    bool operator>=(const uInt& n) const {
        #if PERFORMANCE_TEST
            START_TEST(GTE_TIME)
        #endif
        if (this->bits.size() > n.bits.size())
            return true;
        else if (this->bits.size() < n.bits.size())
            return false;
        for (uint64_t i = this->bits.size(); i != uint64_t(-1); --i) {
            if (this->bits[i] ^ n.bits[i])
                return this->bits[i];
        }
        #if PERFORMANCE_TEST
            END_TEST(GTE_TIME)
        #endif
        return true;
    }
};


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



std::string uInt::to_string() const {
    #if PERFORMANCE_TEST
        START_TEST(UINT_TO_STRING_TIME)
    #endif
    if (*this == 0) return std::string("0");
    uInt n(*this), mod;
    std::string result("");
    while (n != ZERO) {
        std::pair<std::vector<bool>, std::vector<bool>> div_mod_result = n.div_and_mod(TEN);
        mod.bits = div_mod_result.second;
        n.bits = div_mod_result.first;
        if (mod == ZERO)
            result = '0' + result;
        else if (mod == ONE)
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
    #if PERFORMANCE_TEST
        END_TEST(UINT_TO_STRING_TIME)
    #endif
    return result;
}


std::string uInt::to_string(const uint64_t& base) const {
    std::string result_str;
    if (base == 2u) {
        for (auto it = this->bits.rbegin(); it != this->bits.rend(); ++it) {
            result_str.append(1, *it ? '1' : '0');
        }
        return result_str;
    }
    if (base == 10u) {
        return this->to_string();
    }
    throw std::runtime_error("ERROR: Unaccepted base");
}


uInt& uInt::operator++() {
    return *this += ONE;
}
uInt& uInt::operator--() {
    return *this -= ONE;
}


// ========================== Non-Class Functions ========================== //
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


// ========================== Performance Testing ========================== //
#if PERFORMANCE_TEST
void print_performance_test_results() {
    double total = 0.0;
    std::sort(durations.begin(), durations.end(), std::greater<TEST_RESOLUTION>());
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