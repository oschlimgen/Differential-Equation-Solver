#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>



class Fraction {
  typedef uint64_t int_t;
  typedef uint8_t sint_t;

public:
  constexpr Fraction();
  constexpr Fraction(int_t number);

  constexpr Fraction(int n);
  constexpr Fraction(double n);

  friend constexpr Fraction operator +(const Fraction& obj);
  friend constexpr Fraction operator -(const Fraction& obj);

  friend constexpr Fraction operator *(const Fraction& lhs, const Fraction& rhs);
  friend constexpr Fraction operator /(const Fraction& lhs, const Fraction& rhs);

  friend constexpr bool operator ==(const Fraction& lhs, const Fraction& rhs);
  
  void reduce();

  constexpr explicit operator int_t() const;
  constexpr explicit operator int() const;
  constexpr explicit operator double() const;
  
  std::string toString() const;


private:
  int_t numerator;
  int_t denominator;
  sint_t sign;
  
  static constexpr int_t zeroInt = 0;
  static constexpr int_t oneInt = 1;
  static constexpr int_t maxInt = std::numeric_limits<int_t>::max();

  static constexpr sint_t intSize = 64;
  static constexpr sint_t positive = 0b00000000;
  static constexpr sint_t negative = 0b10000000;

  static constexpr sint_t signBit = 0b10000000;
  static constexpr sint_t expBits = 0b01111111;
  static constexpr sint_t expSignBit   = 0b01000000;
  static constexpr sint_t expValueBits = 0b00111111; // 2^((n^2+n)/2) multiplier
  
  static constexpr int_t rightHalf = 0x00000000ffffffff;
  static constexpr int_t leftHalf  = 0xffffffff00000000;

  static constexpr int_t shift(int_t num, uint8_t distance);
  static constexpr int_t exp2(sint_t power);
  static constexpr sint_t log2(int_t num);
  static constexpr int_t gcd(int_t a, int_t b);

  /*
   * Computes the value (a * b) / (2 ^ s), with s chosen so that the result
   * of this computation doesn't exceed the integer limit.
   */
  static constexpr int_t scaledProduct(int_t a, int_t b, sint_t s);

  constexpr Fraction(int_t numerator, int_t denominator, sint_t sign);
};

