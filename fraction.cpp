#include "fraction.h"

#include <cmath>


Fraction::Fraction(const uint64_t n) : numerator(n), denominator(1), sign(false) {}
Fraction::Fraction(const int64_t n) : numerator(static_cast<uint64_t>(n < 0 ? -n : n)),
    denominator(1), sign(n < 0) {}

Fraction::Fraction(const int n) : numerator(
    static_cast<uint64_t>(n < 0 ? -n : n)), denominator(1), sign(n < 0) {}

Fraction::Fraction(double n) : sign(n < 0) {
  n = (n < 0.0 ? -n : n);

  if(n > Fraction::maxInt) {
    throw std::overflow_error("Double is too large to be represented as a fraction.");
  }

  uint64_t wholeNumber = static_cast<uint64_t>(n);
  double decimal = n - floor(n);

  uint8_t precision = (uint8_t)63 - log2(wholeNumber);
  denominator = exp2(precision);
  numerator = wholeNumber * denominator;

  double step = 0.5;
  for(uint8_t i = precision - 1; i < precision; --i) {
    if(decimal >= step) {
      decimal -= step;
      numerator += exp2(i);
    }
    step /= 2;
  }

  reduce();
}

Fraction Fraction::operator*(uint64_t multiplier) const {
  if(multiplier == zeroInt) {
    return Fraction(zeroInt, oneInt, false);
  }

  uint64_t commonFactor = Fraction::gcd(denominator, multiplier);
  Fraction result { multiplier / commonFactor, denominator / commonFactor, sign };

  if(numerator > Fraction::maxInt / result.numerator) {
    uint8_t simplifier = (int)log2(numerator) + (int)log2(result.numerator) - 64;

    uint64_t xl = (numerator & leftHalf) >> 32;
    uint64_t xr = (numerator & rightHalf);
    uint64_t yl = (result.numerator & leftHalf) >> 32;
    uint64_t yr = (result.numerator & rightHalf);

    result.numerator = ((xl * yl) << ((uint8_t)64 - simplifier)) + shift(((xl * yr) + (xr * yl) +
        ((xr * yr) >> 32)), (32 - static_cast<int>(simplifier)));
    result.denominator = result.denominator >> simplifier;

    if(result.denominator == 0) {
      throw std::overflow_error("Number too big to fit in fraction.");
    }
    return result;
  }

  result.numerator *= numerator;
  return result;
}
// Fraction Fraction::operator*(const Fraction& other) {
//   numerator *= other.numerator;
//   denominator *= other.denominator;
//   sign = (sign ^ other.sign);
//   reduce();
//   return *this;
// }

// Fraction Fraction::operator/(const uint64_t divisor) {
//   uint64_t commonFactor = Fraction::gcd(numerator, divisor);
//   numerator /= commonFactor;
//   denominator = denominator * (divisor / commonFactor);
//   return *this;
// }
// Fraction Fraction::operator/(const Fraction& other) {
//   numerator *= other.denominator;
//   denominator *= other.numerator;
//   sign = (sign ^ other.sign);
//   reduce();
//   return *this;
// }

Fraction::operator uint64_t() const {
  return (numerator / denominator);
}
Fraction::operator int() const {
  constexpr uint64_t maxInt16 = std::numeric_limits<int>::max();
  if(numerator > maxInt16 || denominator > maxInt16) {
    uint64_t numeratorCopy = numerator;
    uint64_t denominatorCopy = denominator;
    do {
      numeratorCopy /= 2;
      denominatorCopy /= 2;
    } while(numeratorCopy > maxInt16 || denominatorCopy > maxInt16);
    return (sign ? -1 : 1) * (static_cast<int>(numeratorCopy) / static_cast<int>(denominatorCopy));
  }
  return (sign ? -1 : 1) * (static_cast<int>(numerator) / static_cast<int>(denominator));
}
Fraction::operator double() const {
  return (sign ? -1 : 1) * (static_cast<double>(numerator) / denominator);
}

std::string Fraction::toString() const {
  return (sign ? "-" : "") + std::to_string(numerator) + '/' + std::to_string(denominator);
}



constexpr uint64_t Fraction::shift(const uint64_t num, const int dist) {
  if(dist < 0) {
    return (num >> -dist);
  }
  return (num << dist);
}

constexpr uint64_t Fraction::exp2(const uint8_t power) {
  return (oneInt << power);
}

constexpr uint8_t Fraction::log2(uint64_t n) {
  uint8_t precision = 0;
  while(n > 0) {
    n = n >> 1;
    ++precision;
  }
  return precision;
}

constexpr uint64_t Fraction::gcd(uint64_t a, uint64_t b) {
  uint64_t c = a % b;
  while(c != 0) {
    // Shift series to next terms
    a = b;
    b = c;
    // Compute next term of series
    c = a % b;
  }
  return b;
}

Fraction::Fraction(uint64_t numerator, uint64_t denominator, bool sign) :
    numerator(numerator), denominator(denominator), sign(sign) {}

void Fraction::reduce() {
  uint64_t commonFactor = Fraction::gcd(numerator, denominator);
  numerator /= commonFactor;
  denominator /= commonFactor;
}


