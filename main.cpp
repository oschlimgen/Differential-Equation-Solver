#include <iostream>
#include <iomanip>
#include <bitset>
#include <limits>

#include "fraction.cpp"

#define printPrecise std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10)


constexpr uint64_t exp2(const int power) {
  return ((uint64_t)1 << power);
}

constexpr uint8_t log2(uint64_t n) {
  uint8_t precision = 0;
  while(n > 0) {
    n = n >> 1;
    ++precision;
  }
  return precision;
}


int main() {
  // constexpr uint64_t rightHalf = 0x00000000ffffffff;
  // constexpr uint64_t leftHalf  = 0xffffffff00000000;
  // const uint64_t numerator = exp2(40);

  // const uint64_t num2 = exp2(30) + exp2(25);
  
  // constexpr uint64_t xl = (numerator & leftHalf) >> 32;
  // constexpr uint64_t xr = (numerator & rightHalf);
  // constexpr uint64_t yl = (num2 & leftHalf) >> 32;
  // constexpr uint64_t yr = (num2 & rightHalf);

  // constexpr uint8_t simplifier = log2(numerator) + log2(num2) - 65;
  // constexpr uint64_t remaining = ((xl * yl) << (64 - simplifier)) + (((xl * yr) + (xr * yl) +
  //       ((xr * yr) >> 32)) << (32 - simplifier));
  // constexpr uint64_t result = remaining >> 55;

  // for(uint64_t num2 = 1; num2 < exp2(63); num2 *= 2) {
  //   uint64_t simplifier = log2(numerator) + log2(num2) - 65;
  //   if(simplifier == 64) {
  //     throw std::overflow_error("Multiplication doesn't fit in fraction.");
  //   }
  //   uint64_t xl = (numerator & leftHalf) >> 32;
  //   uint64_t xr = (numerator & rightHalf);
  //   uint64_t yl = (num2 & leftHalf) >> 32;
  //   uint64_t yr = (num2 & rightHalf);

  //   uint64_t remaining = ((xl * yl) << (64 - simplifier)) + (((xl + xr) * (yl + yr) - (xl * yl) - (xr * yr) +
  //       ((xr * yr) >> 32)) << (32 - simplifier));

  //   std::cout << numerator << " * " << num2 << " = " << exp2((int)simplifier) << " * " << remaining << std::endl;
  // }

  Fraction one = 315.0 / 26.0;
  std::cout << one.toString() << std::endl;
  std::cout << printPrecise << (double)one << std::endl;

  Fraction result2 = one * 50100060000001;
  std::cout << result2.toString() << std::endl;
  std::cout << printPrecise << (double)result2 << std::endl;
  std::cout << printPrecise << (315.0 / 26.0) * 50100060000001 << std::endl;

  std::cout << std::numeric_limits<uint64_t>::max() << std::endl;
  std::cout << std::numeric_limits<double>::max() << std::endl;

  double decimal = 1.0/3.0;
  Fraction frac(decimal);
  std::cout << frac.toString() << std::endl;
  std::cout << printPrecise << (double)frac << std::endl;
  std::cout << printPrecise << decimal << std::endl;


  // while(true) {
  //   double numDouble;
  //   std::cin >> numDouble;
  //   uint64_t numInt = reinterpret_cast<uint64_t&>(numDouble);
  //   std::bitset<64> numBits = numInt;
  //   std::cout << numBits << std::endl;
  // }

  return 0;
}

