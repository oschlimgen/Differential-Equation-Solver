#include "fraction.h"

#include <cmath>



constexpr Fraction::int_t Fraction::shift(const int_t num, const uint8_t distance) {
  if(distance & 0b10000000) {
    return (num >> (0b10000000 - (distance & 0b01111111)));
  }
  return (num << distance);
}

constexpr Fraction::int_t Fraction::exp2(const sint_t power) {
  return (oneInt << power);
}

constexpr Fraction::sint_t Fraction::log2(int_t n) {
  sint_t precision = 0;
  while(n > 0) {
    n = n >> 1;
    ++precision;
  }
  return precision;
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

    // Fancy formula to compute (numerator * result.numerator / exp2(simplifier)) without overflow
    //    x * y       = (xl * 2^32 + xr) * (yl * 2^32 + yr)
    //                = (xl * yl) * 2^64 + (xl * yr + xr * yl) * 2^32 + (xr * yr)
    //    x * y / 2^s = (xl * yl) * 2^(64-s) + (xl * yr + xr * yl) * 2^(32-s) + (xr * yr) * 2^(-s)
    return ((xl * yl) << (intSize - s)) +
        shift((xl * yr) + (xr * yl), (intSize / 2) - s) +
        ((xr * yr) >> s);
}

constexpr Fraction::Fraction(int_t numerator, int_t denominator, sint_t sign) :
    numerator(numerator), denominator(denominator), sign(sign) {}




constexpr Fraction::Fraction() : numerator(oneInt), denominator(oneInt), sign(positive) {}

constexpr Fraction::Fraction(const int_t n) : numerator(n), denominator(oneInt), sign(positive) {}

constexpr Fraction::Fraction(const int n) : numerator(
    static_cast<int_t>(n < 0 ? -n : n)), denominator(oneInt), sign(n < 0 ? negative : positive) {}

constexpr Fraction::Fraction(const double n) : numerator(
    static_cast<int_t>((n < 0.0 ? -n : n) * exp2(intSize - log2(static_cast<int_t>(n < 0.0 ? -n : n)) - log2(static_cast<int_t>(n < 0.0 ? -1.0/n : 1.0/n))))),
    denominator(exp2(intSize - log2(static_cast<int_t>(n < 0.0 ? -n : n)) - log2(static_cast<int_t>(n < 0.0 ? -1.0/n : 1.0/n)))), sign(n < 0.0 ? negative : positive) {}


constexpr Fraction operator +(const Fraction& obj) {
  Fraction::int_t commonFactor = Fraction::gcd(obj.numerator, obj.denominator);
  return { obj.numerator / commonFactor, obj.denominator / commonFactor, obj.sign};
}

constexpr Fraction operator -(const Fraction& obj) {
  return { obj.numerator, obj.denominator, static_cast<Fraction::sint_t>(obj.sign ^ Fraction::negative) };
}

constexpr Fraction operator *(const Fraction& lhs, const Fraction& rhs) {
  if(lhs.numerator == Fraction::zeroInt || rhs.numerator == Fraction::zeroInt) {
    return Fraction { Fraction::zeroInt, Fraction::oneInt, Fraction::positive };
  }

  Fraction result;
  Fraction::sint_t numeratorOrder = Fraction::log2(lhs.numerator) + Fraction::log2(rhs.numerator) - 1;
  Fraction::sint_t denominatorOrder = Fraction::log2(lhs.denominator) + Fraction::log2(rhs.denominator) - 1;

  // Calculate the numerator of the resulting fraction
  Fraction::sint_t simplifier = 0;
  if(numeratorOrder > Fraction::intSize) {
    simplifier = numeratorOrder - Fraction::intSize + 1;
    result.numerator = Fraction::scaledProduct(lhs.numerator, rhs.numerator, simplifier);

    if(result.denominator == 0) {
      throw std::overflow_error("Number too big to fit in fraction.");
    }
  }
  else {
    result.numerator = lhs.numerator * rhs.numerator;
  }

  // Calculate the denominator of the resulting fraction
  if(denominatorOrder > Fraction::intSize + simplifier) {
    // Fraction must be simplified further before multiplication
    Fraction::sint_t denominatorSimplifier = denominatorOrder - (Fraction::intSize + simplifier) + 1;
    result.denominator = Fraction::scaledProduct(lhs.denominator, rhs.denominator, denominatorSimplifier);
    // Update numerator with the simplifier shift
    result.numerator = (result.numerator >> denominatorSimplifier);
  }
  else if(denominatorOrder > Fraction::intSize) {
    // Fraction doesn't need further simplification, but scaledProduct must be used to prevent overflow
    result.denominator = Fraction::scaledProduct(lhs.denominator, rhs.denominator, simplifier);
  }
  else if(simplifier != 0) {
    result.denominator = (lhs.denominator * rhs.denominator) >> simplifier;
  }
  else {
    result.denominator = lhs.denominator * rhs.denominator;
  }

  result.sign = (lhs.sign & Fraction::signBit) ^ (rhs.sign & Fraction::signBit);
  
  if(result.denominator == 0) {
    throw std::overflow_error("Product too big to fit in fraction.");
  }

  return result;
}

constexpr Fraction operator /(const Fraction& lhs, const Fraction& rhs) {
  Fraction reciprical { rhs.denominator, rhs.numerator, static_cast<Fraction::sint_t>(rhs.sign ^ Fraction::expSignBit) };
  return operator*(lhs, reciprical);
}

constexpr bool operator ==(const Fraction& lhs, const Fraction& rhs) {
  return (lhs.numerator == rhs.numerator) && (lhs.denominator == rhs.denominator) &&
      ((lhs.sign & Fraction::signBit) == (rhs.sign & Fraction::signBit));
}


void Fraction::reduce() {
  int_t commonFactor = Fraction::gcd(numerator, denominator);
  numerator /= commonFactor;
  denominator /= commonFactor;
}

constexpr Fraction::operator int_t() const {
  return static_cast<int64_t>(numerator / denominator) * (sign ? -1 : 1);
}

constexpr Fraction::operator int() const {
  constexpr sint_t realIntSize = 8 * sizeof(int) - 1;
  const sint_t numeratorOrder = log2(numerator);
  const sint_t denominatorOrder = log2(denominator);
  if(numeratorOrder > realIntSize || denominatorOrder > realIntSize) {
    const sint_t simplifier = (numeratorOrder > denominatorOrder ? numeratorOrder : denominatorOrder) - realIntSize;
    const int modifiedDenominator = static_cast<int>(denominator >> simplifier);

    if(modifiedDenominator == zeroInt) {
      throw std::overflow_error("Fraction is too large to be expressed as an int.");
    }
    return ((sign & signBit) ? -1 : 1) * (static_cast<int>(numerator >> simplifier) / modifiedDenominator);
  }
  return ((sign & signBit) ? -1 : 1) * (static_cast<int>(numerator) / static_cast<int>(denominator));
}

constexpr Fraction::operator double() const {
  return ((sign & signBit) ? -1 : 1) * (static_cast<double>(numerator) / denominator);
}

std::string Fraction::toString() const {
  return ((sign & signBit) && (numerator != 0) ? "-" : "") + std::to_string(numerator) + '/' + std::to_string(denominator);
}



