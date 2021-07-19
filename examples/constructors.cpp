#include "../src/uInt.hpp"
#include <iostream>

int main(int argc, char** argv) {
    atn::uInt foo;
    std::cout << "1. Value of foo: " << foo << std::endl;
    foo = 100;
    std::cout << "2. Value of foo: " << foo << std::endl;
    foo = atn::uInt("92750475496957904");
    std::cout << "3a. Value of foo: " << foo << std::endl;
    foo = atn::uInt("0xDEADBEEF");
    std::cout << "3b. Value of foo: " << foo << std::endl;
    foo = atn::uInt("0b1010001011110100101010101000101110");
    std::cout << "3c. Value of foo: " << foo << std::endl;
}