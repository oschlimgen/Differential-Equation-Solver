#include <iostream>
#include <iomanip>
#include <bitset>
#include <limits>

#include "recurrence.cpp"

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

constexpr Fraction nextTerm(const Fraction& nMinusOne, const Fraction& nMinusTwo, const int n) {
  return Fraction();
}


int main() {
  // constexpr Fraction zero { 0 };
  // constexpr Fraction one { 1 };

  // constexpr RecursiveSeries series(&nextTerm, zero, one);

  // const int n = series.getN();
  // const Fraction term = series.getTerm();

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

  // constexpr Fraction cfrac { Fraction { 5 } * 3 };
  constexpr Fraction cfrac { +((Fraction() * -80) / -15) };
  constexpr double cdec { 315.0 / 26.0 };

  constexpr Fraction one { 26.0 / 315.0 };
  std::cout << one.toString() << std::endl;
  std::cout << printPrecise << static_cast<uint64_t>(one) << std::endl;

  constexpr Fraction two = (uint64_t)50100060000001;
  constexpr Fraction three = -1.0;

  constexpr Fraction result = one / two * three;
  constexpr Fraction result2 = +result;
  std::cout << result2.toString() << std::endl;
  std::cout << printPrecise << ((double)result2) << std::endl;
  std::cout << printPrecise << ((26.0 / 315.0) / 50100060000001) << std::endl;

  constexpr Fraction result3 = +((1 / result) / two / three);
  constexpr double val = (double)result3;

  constexpr bool same = (result == result2);

  // constexpr uint64_t a = 1912043986730614784;
  // constexpr uint64_t b = 270991224540005409;

  // constexpr bool c = b;
  
  // constexpr uint8_t realIntSize = 8 * sizeof(int) - 1;
  // const uint8_t numeratorOrder = log2(a);
  // const uint8_t denominatorOrder = log2(b);

  // const uint8_t simplifier = (numeratorOrder > denominatorOrder ? numeratorOrder : denominatorOrder) - realIntSize;
  // const int modifiedDenominator = static_cast<int>(b >> simplifier);
  // const int modifiedNumerator = static_cast<int>(a >> simplifier);
  // const int r = modifiedNumerator / modifiedDenominator;

  // constexpr int simplification = static_cast<int>((result3));
  // std::cout << simplification << std::endl;
  // Fraction modifiable = result;
  // modifiable = modifiable * result;

  // std::cout << std::numeric_limits<uint64_t>::max() << std::endl;
  // std::cout << std::numeric_limits<double>::max() << std::endl;

  // double decimal = 1.0/3.0;
  // Fraction frac(decimal);
  // std::cout << frac.toString() << std::endl;
  // std::cout << printPrecise << (double)frac << std::endl;
  // std::cout << printPrecise << decimal << std::endl;


  // while(true) {
  //   double numDouble;
  //   std::cin >> numDouble;
  //   uint64_t numInt = reinterpret_cast<uint64_t&>(numDouble);
  //   std::bitset<64> numBits = numInt;
  //   std::cout << numBits << std::endl;
  // }

  return 0;
}

