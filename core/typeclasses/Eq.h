#ifndef FPCPP_CORE_TYPECLASSES_EQ_H
#define FPCPP_CORE_TYPECLASSES_EQ_H

#include <concepts>
#include <string>
#include <core/typeclasses/Ord.h>
#include <core/functional/Functor.h>
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

template<IsStringType A, IsStringType B>
bool Equal(const A& a, const B& b) {
  const std::string aStr = a, bStr = b;
  return Eq<std::string>::equal(aStr, bStr);
}

template<
  typename FA, typename FB,
  typename A = typename FA::ValueType,
  typename B = typename FB::ValueType
> requires IsStringType<A> && IsStringType<B>
bool Equal(const FA& fa, const FB& fb) {
  using MappedF = typename FA::template NewType<std::string>;

  const MappedF mappedA = Functor::Map(fa, [](auto a) { return std::string(a); });
  const MappedF mappedB = Functor::Map(fb, [](auto b) { return std::string(b); });

  return Eq<MappedF>::equal(mappedA, mappedB);
}


#endif // FPCPP_CORE_TYPECLASSES_EQ_H