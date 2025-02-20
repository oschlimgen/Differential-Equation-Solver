#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>


class Fraction {
public:
  Fraction(uint64_t number);
  Fraction(int64_t number);

  Fraction(int n);
  Fraction(double n);

  Fraction operator*(uint64_t multiplier) const;

  explicit operator uint64_t() const;
  explicit operator int() const;
  explicit operator double() const;
  
  std::string toString() const;


private:
  uint64_t numerator;
  uint64_t denominator;
  bool sign;
  
  static constexpr uint64_t zeroInt = 0;
  static constexpr uint64_t oneInt = 1;
  static constexpr uint64_t maxInt = std::numeric_limits<uint64_t>::max();

  static constexpr uint64_t rightHalf = 0x00000000ffffffff;
  static constexpr uint64_t leftHalf  = 0xffffffff00000000;

  static constexpr uint64_t shift(uint64_t num, int dist);
  static constexpr uint64_t exp2(uint8_t power);
  static constexpr uint8_t log2(uint64_t num);
  static constexpr uint64_t gcd(uint64_t a, uint64_t b);

  Fraction(uint64_t numerator, uint64_t denominator, bool sign);

  void reduce();
};
