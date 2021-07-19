#include "../src/uInt.hpp"
#include <iostream>

int main(int argc, char** argv) {
    srand(0);
    uint64_t rand1 = rand();
    uint64_t rand2 = rand();
    std::cout << rand1 << ", " << rand2 << std::endl;
    atn::uInt a = rand1; // 1804289383
    std::cout << a << std::endl;
    atn::uInt b = rand2; // 846930886
    std::cout << b << std::endl;
    atn::uInt c = a + b; // 2651220269
    std::cout << c << std::endl;
    atn::uInt d = a - b; // 957358497
    std::cout << d << std::endl;
    atn::uInt e = a * b; // 1.528e18
    std::cout << e << std::endl;
    atn::uInt f = e / b; // 1804289383
    std::cout << f << std::endl;
    atn::uInt g = a % b; // 110427611
    std::cout << g << std::endl;
    atn::uInt h = b - a; // 0
    std::cout << h << std::endl;
    atn::uInt i = a << 3; // 14434315064
    std::cout << i << std::endl;
    atn::uInt j = a >> 3; // 225536172
    std::cout << j << std::endl;
    atn::uInt k = a & b; // 571146566
    std::cout << k << std::endl;
    atn::uInt l = a | b; // 2080073703
    std::cout << l << std::endl;
    atn::uInt m = a ^ b; // 1508927137
    std::cout << m << std::endl;
}
