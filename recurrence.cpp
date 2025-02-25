#include "recurrence.hpp"

#include "fraction.cpp"


constexpr RecursiveSeries::RecursiveSeries(
      Fraction (*relation)(const Fraction& nMinusOne, const Fraction& nMinusTwo, int n),
      const Fraction& zerothTerm, const Fraction& firstTerm) : 
      relation(relation), evenTerm(zerothTerm), oddTerm(firstTerm), n(1) {}

constexpr int RecursiveSeries::getN() const {
  return n;
}
constexpr const Fraction& RecursiveSeries::getTerm() const {
  if(n % 2 == 0) {
    return evenTerm;
  }
  else {
    return oddTerm;
  }
}


constexpr void RecursiveSeries::computeNext() {
  n++;
  if(n % 2 == 0) {
    evenTerm = (*relation)(oddTerm, evenTerm, n);
  }
  else {
    oddTerm = (*relation)(evenTerm, oddTerm, n);
  }
}
