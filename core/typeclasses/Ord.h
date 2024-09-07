#pragma once

#include <concepts>

template<typename A, typename = void>
struct Ord;

template<typename A>
concept HasOrd = requires(const A& a, const A& b) {
  { Ord<A>::compare(a, b) } -> std::same_as<int>;
};

template<typename A> requires HasOrd<A>
int Compare(const A& a, const A& b) {
  return Ord<A>::compare(a, b);
}

template<typename A>
struct Ord<A, std::enable_if_t<std::is_arithmetic_v<A>>> {
  static int compare(const A& a, const A& b) {
    return
      a < b ? -1 :
      a > b ? 1 :
      0;
  }
};


