#ifndef FPCPP_CORE_TYPECLASSES_NUM_H
#define FPCPP_CORE_TYPECLASSES_NUM_H

#include <concepts>
#include <core/data/Concepts.h>

template<typename A>
struct Num;

template<typename A>
concept HasNum =  requires(const A& a, const A& b) {
  { Num<A>::add(a, b) } -> std::same_as<A>;
  { Num<A>::subtract(a, b) } -> std::same_as<A>;
  { Num<A>::multiply(a, b) } -> std::same_as<A>;
  { Num<A>::divide(a, b) } -> std::same_as<A>;
  { Num<A>::mod(a, b) } -> std::same_as<A>;
};

template<IsArithmetic A>
struct Num<A> {
  static A add(const A& a, const A& b) { return a + b; }
  static A subtract(const A& a, const A& b) { return a - b; }
  static A multiply(const A& a, const A& b) { return a * b; }
  static A divide(const A& a, const A& b) { return a / b; }
  static A mod(const A& a, const A& b) { return a % b; }
};

#endif // FPCPP_CORE_TYPECLASSES_NUM_H