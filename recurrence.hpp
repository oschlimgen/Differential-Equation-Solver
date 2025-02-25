#pragma once

#include "fraction.hpp"


class RecursiveSeries {
public:
  constexpr RecursiveSeries(Fraction (*relation)(const Fraction& nMinusOne, const Fraction& nMinusTwo, int n),
      const Fraction& zerothTerm, const Fraction& firstTerm);

  constexpr int getN() const;
  constexpr const Fraction& getTerm() const;

  constexpr void computeNext();

private:
  Fraction (*relation)(const Fraction&, const Fraction&, int);
  Fraction evenTerm;
  Fraction oddTerm;
  int n;
};
