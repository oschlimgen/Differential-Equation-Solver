#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>


class Fraction {
  typedef uint64_t int_t;
  typedef int sint_t;

public:
  constexpr Fraction();
  constexpr Fraction(int_t numerator, int_t denominator);
  constexpr Fraction(int_t numerator, int_t denominator, bool negative);

  constexpr Fraction(int n);
  constexpr Fraction(double n);

  friend constexpr Fraction operator +(const Fraction& obj);
  friend constexpr Fraction operator -(const Fraction& obj);

  friend constexpr Fraction operator *(const Fraction& lhs, const Fraction& rhs);
  friend constexpr Fraction operator /(const Fraction& lhs, const Fraction& rhs);

  friend constexpr bool operator ==(const Fraction& lhs, const Fraction& rhs);

  constexpr explicit operator int_t() const;
  constexpr explicit operator int() const;
  constexpr explicit operator double() const;
  
  std::string toString() const;


private:
  int_t numerator;
  int_t denominator;
  sint_t exponent;
  bool negative;
  
  static constexpr int_t oneInt = 1;
  static constexpr int_t maxInt = -1;
  static constexpr sint_t intSize = 64;

  // static constexpr sint_t signBit      = 0b01000000000000000000000000000000;
  // static constexpr sint_t expBits      = 0b10111111111111111111111111111111;
  // static constexpr sint_t expSignBit   = 0b10000000000000000000000000000000;
  // static constexpr sint_t expValueBits = 0b00111111111111111111111111111111;
  
  // static constexpr sint_t positive     = 0b00000000000000000000000000000000;
  // static constexpr sint_t negative     = 0b01000000000000000000000000000000;
  // static constexpr sint_t expNegative  = 0b10000000000000000000000000000000;
  // static constexpr sint_t infinity     = 0b10000000000000000000000000000000;

  static constexpr int_t rightHalf = 0x00000000ffffffff;
  static constexpr int_t leftHalf  = 0xffffffff00000000;

  static constexpr int_t shift(int_t num, uint8_t distance);
  static constexpr int_t exp2(sint_t power);
  static constexpr double exp2d(sint_t power);
  static constexpr sint_t log2(int_t num);
  static constexpr int_t gcd(int_t a, int_t b);

  /*
   * Computes the value (a * b) / (2 ^ s), with s chosen so that the result
   * of this computation doesn't exceed the integer limit.
   */
  static constexpr int_t scaledProduct(int_t a, int_t b, sint_t s);

  constexpr Fraction(int_t numerator, int_t denominator, sint_t exponent, bool negative);
};


