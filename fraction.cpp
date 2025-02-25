#include "fraction.hpp"

#include <cmath>

#include "compiledcalculations/exponentlookup.h"


#define leadingZeros __builtin_clzll
#define trailingZeros __builtin_ctzll



constexpr Fraction::int_t Fraction::shift(const int_t num, const uint8_t distance) {
  if(distance & 0b10000000) {
    return (num >> (0b10000000 - (distance & 0b01111111)));
  }
  return (num << distance);
}

constexpr Fraction::int_t Fraction::exp2(const sint_t power) {
  return (oneInt << power);
}

constexpr double Fraction::exp2d(const sint_t power) {
  if(std::is_constant_evaluated()) {
    if(power >= 0) {
      if(power < maxInt) {
        return static_cast<double>(oneInt << power);
      }
      else {
        double halfExp = exp2d(power >> 1);
        if(power & 1) {
          return 2.0 * halfExp * halfExp;
        }
        else {
          return halfExp * halfExp;
        }
      }
    }
    else {
      double halfExp = exp2d(power / 2);
      if(power & 1) {
        return 0.5 * halfExp * halfExp;
      }
      else {
        return halfExp * halfExp;
      }
    }
  }
  return pow(2.0, power);
}

constexpr Fraction::sint_t Fraction::log2(const int_t n) {
  return (intSize - leadingZeros(n));
}

constexpr Fraction::int_t Fraction::gcd(int_t a, int_t b) {
  int_t c = a % b;
  while(c != 0) {
    // Shift series to next terms
    a = b;
    b = c;
    // Compute next term of series
    c = a % b;
  }
  return b;
}



constexpr Fraction::int_t Fraction::scaledProduct(const int_t x, const int_t y, const sint_t s) {
  // Split 64-bit numbers into 32-bit numbers to prevent overflow when multiplying
    int_t xl = (x & leftHalf) >> 32;
    int_t xr = (x & rightHalf);
    int_t yl = (y & leftHalf) >> 32;
    int_t yr = (y & rightHalf);

    // Fancy formula to compute (numerator * result.numerator / exp2(exponent)) without overflow
    //    x * y       = (xl * 2^32 + xr) * (yl * 2^32 + yr)
    //                = (xl * yl) * 2^64 + (xl * yr + xr * yl) * 2^32 + (xr * yr)
    //    x * y / 2^s = (xl * yl) * 2^(64-s) + (xl * yr + xr * yl) * 2^(32-s) + (xr * yr) * 2^(-s)
    return ((xl * yl) << (intSize - s)) +
        shift((xl * yr) + (xr * yl), (intSize / 2) - s) +
        ((xr * yr) >> s);
}

constexpr Fraction::Fraction(int_t numerator, int_t denominator, sint_t exponent, bool negative) :
    numerator(numerator), denominator(denominator), exponent(exponent), negative(negative) {}




constexpr Fraction::Fraction() : numerator(oneInt), denominator(oneInt),
    exponent(0), negative(false) {}

constexpr Fraction::Fraction(const int_t numerator, const int_t denominator) :
    numerator(numerator), denominator(denominator), exponent(0), negative(false) {}

constexpr Fraction::Fraction(const int_t numerator, const int_t denominator, bool negative) :
    numerator(numerator), denominator(denominator), exponent(0), negative(negative) {}


constexpr Fraction::Fraction(const int n) : numerator(static_cast<int_t>
    (n < 0 ? -n : n)), denominator(oneInt), exponent(0), negative(n < 0) {}

constexpr Fraction::Fraction(const double n) : numerator(static_cast<int_t>
    ((n < 0.0 ? -n : n) * exp2(intSize - log2(static_cast<int_t>(n < 0.0 ? -n : n)) - log2(static_cast<int_t>(n < 0.0 ? -1.0/n : 1.0/n))))),
    denominator(exp2(intSize - log2(static_cast<int_t>(n < 0.0 ? -n : n)) - log2(static_cast<int_t>(n < 0.0 ? -1.0/n : 1.0/n)))),
    exponent(0), negative(n < 0.0) {}


constexpr Fraction operator +(const Fraction& obj) {
  if(obj.numerator == 0) {
    return Fraction { 0, 1 };
  }
  Fraction result = obj;

  // Reduce fraction by putting powers of two into exponent
  const int numeratorShift = trailingZeros(result.numerator);
  const int denominatorShift = trailingZeros(result.denominator);
  result.numerator >> numeratorShift;
  result.denominator >> denominatorShift;
  result.exponent += (numeratorShift - denominatorShift);

  // Reduce fraction by cancelling common factors
  Fraction::int_t commonFactor = Fraction::gcd(result.numerator, result.denominator);
  result.numerator /= commonFactor;
  result.denominator /= commonFactor;

  return result;
}

constexpr Fraction operator -(const Fraction& obj) {
  Fraction result = obj;
  result.negative = !result.negative;
  return result;
}

constexpr Fraction operator *(const Fraction& lhs, const Fraction& rhs) {
  if(lhs.numerator == 0 || rhs.numerator == 0) {
    return Fraction { 0, 1 };
  }

  Fraction result;
  result.exponent = rhs.exponent + lhs.exponent;
  const Fraction::sint_t numeratorOrder = Fraction::log2(lhs.numerator) + Fraction::log2(rhs.numerator) - 1;
  const Fraction::sint_t denominatorOrder = Fraction::log2(lhs.denominator) + Fraction::log2(rhs.denominator) - 1;

  // Calculate the numerator of the resulting fraction
  if(numeratorOrder <= Fraction::intSize) {
    result.numerator = lhs.numerator * rhs.numerator;
  }
  else {
    const Fraction::sint_t lhShift = trailingZeros(lhs.numerator);
    const Fraction::sint_t rhShift = trailingZeros(rhs.numerator);
    if(numeratorOrder - (lhShift + rhShift) <= Fraction::intSize) {
      result.numerator = (lhs.numerator >> lhShift) * (rhs.numerator >> rhShift);
      result.exponent += (lhShift + rhShift);
    }
    else {
      const Fraction::sint_t numeratorSimplifier = numeratorOrder - (Fraction::intSize - 1);
      result.numerator = Fraction::scaledProduct(lhs.numerator, rhs.numerator, numeratorSimplifier);
      result.exponent += numeratorSimplifier;
    }
  }

  // Calculate the denominator of the resulting fraction
  if(denominatorOrder <= Fraction::intSize) {
    result.denominator = lhs.denominator * rhs.denominator;
  }
  else {
    const Fraction::sint_t lhShift = trailingZeros(lhs.denominator);
    const Fraction::sint_t rhShift = trailingZeros(rhs.denominator);
    if(denominatorOrder - (lhShift + rhShift) <= Fraction::intSize) {
      result.denominator = (lhs.denominator >> lhShift) * (rhs.denominator >> rhShift);
      result.exponent -= (lhShift + rhShift);
    }
    else {
      const Fraction::sint_t denominatorSimplifier = denominatorOrder - (Fraction::intSize - 1);
      result.denominator = Fraction::scaledProduct(lhs.denominator, rhs.denominator, denominatorSimplifier);
      result.exponent -= denominatorSimplifier;
    }
  }

  result.negative = (lhs.negative == rhs.negative);

  return result;
}

constexpr Fraction operator /(const Fraction& lhs, const Fraction& rhs) {
  Fraction reciprical { rhs.denominator, rhs.numerator, -rhs.exponent, rhs.negative };
  return operator*(lhs, reciprical);
}

constexpr bool operator ==(const Fraction& lhs, const Fraction& rhs) {
  return (lhs.numerator == rhs.numerator) && (lhs.denominator == rhs.denominator) &&
      (lhs.exponent == rhs.exponent) && (lhs.negative == rhs.negative);
}


constexpr Fraction::operator int_t() const {
  return static_cast<int_t>(numerator / denominator) * (negative ? -1 : 1);
}

constexpr Fraction::operator int() const {
  constexpr sint_t realIntSize = 8 * sizeof(int) - 1;
  const sint_t numeratorOrder = log2(numerator);
  const sint_t denominatorOrder = log2(denominator);
  if(numeratorOrder > realIntSize || denominatorOrder > realIntSize) {
    const sint_t simplifier = __max(numeratorOrder, denominatorOrder) - realIntSize;
    const int modifiedDenominator = static_cast<int>(denominator >> simplifier);

    if(modifiedDenominator == 0) {
      throw std::overflow_error("Fraction is too large to be expressed as an int.");
    }
    return (negative ? -1 : 1) * (static_cast<int>(numerator >> simplifier) / modifiedDenominator);
  }
  return (negative ? -1 : 1) * (static_cast<int>(numerator) / static_cast<int>(denominator));
}

constexpr Fraction::operator double() const {
  return (negative ? -1 : 1) * exp2d(exponent) * (static_cast<double>(numerator) / denominator);
}

std::string Fraction::toString() const {
  return (negative && (numerator != 0) ? "-" : "") + std::to_string(numerator) + '/' + std::to_string(denominator);
}


