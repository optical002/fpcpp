#ifndef FPCPP_CORE_TYPECLASSES_EQ_H
#define FPCPP_CORE_TYPECLASSES_EQ_H

#include <concepts>
#include <string>
#include <core/typeclasses/Ord.h>
#include <core/data/Concepts.h>
#include <core/data/Tag.h>

template<typename A>
struct Eq;

template<typename A>
concept HasEq = requires(const A& a, const A& b) {
  { Eq<A>::equal(a, b) } -> std::same_as<bool>;
};

template<HasEq A>
bool Equal(const A& a, const A& b) {
  return Eq<A>::equal(a, b);
}

template<IsArithmetic A>
struct Eq<A> {
  static bool equal(const A& a, const A& b) { return a == b; }
};

template<>
struct Eq<bool> {
  static bool equal(const bool& a, const bool& b) { return a == b;}
};

template<HasOrd A> requires (!IsArithmetic<A>)
struct Eq<A> {
  static bool equal(const A& a, const A& b) {
    return Ord<A>::compare(a, b) == 0;
  }
};

template<>
struct Eq<std::string> {
  static bool equal(const std::string& a, const std::string& b) {
    return a == b;
  }
};

template<HasEq... Args>
struct Eq<std::tuple<Args...>> {
  static bool equal(const std::tuple<Args...>& a, const std::tuple<Args...>& b) {
    return std::apply([]<typename... T0>(const T0&... lhs) {
      return (... && Eq<std::decay_t<T0>>::equal(lhs, lhs));
    }, a);
  }
};

template<HasEq A, HasTag TagType>
struct Eq<Tagged<A, TagType>> {
  static bool equal(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) {
    return Equal(a.a(), b.a());
  }
};

#endif // FPCPP_CORE_TYPECLASSES_EQ_H