#pragma once

#include <concepts>
#include <core/data/Concepts.h>

template<typename A>
struct Ord;

template<typename A>
concept HasOrd = requires(const A& a, const A& b) {
  { Ord<A>::compare(a, b) } -> std::same_as<int>;
};

template<HasOrd A>
int Compare(const A& a, const A& b) {
  return Ord<A>::compare(a, b);
}

template<IsArithmetic A>
struct Ord<A> {
  static int compare(const A& a, const A& b) {
    return
      a < b ? -1 :
      a > b ? 1 :
      0;
  }
};


