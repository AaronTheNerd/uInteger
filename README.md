# uInteger
> An unsigned arbitrary precision integer class in C++.

## Usage

Simply include the `uInt.hpp` file to use the `uInt` class. Note that the uInteger class is contained within the `atn` namespace. 

```
#include "uInt.hpp"
#include <iostream>

int main(int argc, char** argv) {
    atn::uInt foo = 100;
    std::cout << "Value of foo: " << foo << std::endl;
}
```

The class acts almost exactly as a traditional integer. With some key differences. For example, the class contains methods of converting a `std::string` into a `uInt`. Additionally, some bitwise operations aren't included.

## Constructors

type        | function definition
----------- | -------------------
default (1) | `uInt();`
integer (2) | `uInt(const uint64_t num&);`
string (3)  | `explicit uInt(const std::string& str);` 
copy(4)     | `uInt(const uInt& n);`
1. Default constructor, defaults to a value of 0.
2. Sets the uInt to a specified integer value.
3. Sets the uInt to a specified value based off a `std::string`. Accepted prefixes: 0x and 0b for hexadecimal and binary string respectively.
4. Copies the value from one `uInt` to another.

Example:
```
#include "uInt.hpp"
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
```

Ouput:
```
1. Value of foo: 0
2. Value of foo: 100
3a. Value of foo: 92750475496957904
3b. Value of foo: 3735928559
3c. Value of foo: 10935773742
```

## Operators

Excluded operators: `&&`, `||`, `!`, `~`. All other operators are fully implemented and are implemented to be identical as a traditional integer.

## Class methods

Currently there is only one class method which is not an overloaded operator: `std::string uInt::to_string() const;` which converts the `uInt` to a `std::string`.

## Examples:
```
#include "uInt.hpp"
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
}
```

Outputs: 
```
1804289383, 846930886
1804289383
846930886
2651220269
957358497
1528108405744583338
1804289383
110427611
0
14434315064
225536172
```

## Optimization History

> All optimization benchmarks are run from `benchmark.cpp`. With a -O3 optimization flag.

First run (7/18/2021):
```
354224848179261915075

N=100, Runtime: 0.584474ms

PERFORMANCE TEST RESULTS
======================================================
          REMOVE_LEAD_ZEROS || 0.066568ms, 7.50569%
                INT TO UINT || 0.085141ms, 9.59983%
             STRING TO UINT ||        0ms, 0%
                       COPY || 0.033467ms, 3.77348%
                   ADDITION || 0.060609ms, 6.8338%
                SUBTRACTION || 0.069762ms, 7.86582%
             MULTIPLICATION || 0.118251ms, 13.3331%
                   DIVISION ||        0ms, 0%
                     MODULO || 0.017856ms, 2.0133%
                 SHIFT LEFT || 0.092823ms, 10.466%
                SHIFT RIGHT ||        0ms, 0%
                BITWISE AND ||        0ms, 0%
                 BITWISE OR ||        0ms, 0%
                BITWISE XOR ||        0ms, 0%
                     EQUALS ||  0.00203ms, 0.228887%
                 NOT EQUALS || 0.000101ms, 0.011388%
                  LESS THAN ||        0ms, 0%
               GREATER THAN ||        0ms, 0%
   GREATER THAN OR EQUAL TO || 0.002441ms, 0.275228%
      LESS THAN OR EQUAL TO || 0.002554ms, 0.287969%
             UINT TO STRING || 0.335298ms, 37.8056%

```

## Future Work

I would like to spend a lot more time optimizing this project. The largest number calculated was the 100,000th term in the fibonacci sequence. This number was 20,899 decimal digits, however, it took about 3 minutes to perform this calculation with -O3.
