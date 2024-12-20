#ifndef FPCPP_CORE_TYPECLASSES_NUM_H
#define FPCPP_CORE_TYPECLASSES_NUM_H

#include <concepts>
#include <core/data/Concepts.h>
#include <core/data/Tag.h>

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
  static A mod(const A& a, const A& b) {
    if constexpr (std::is_integral_v<A>) {
      // For integer types
      return a % b;
    } else {
      // For non-integer types, like float or double, use fmod
      return static_cast<A>(std::fmod(a, b));
    }
  }
};

template<HasNum A, HasTag TagType>
struct Num<Tagged<A, TagType>> {
  static Tagged<A, TagType> add(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) { return Tag<TagType>(Num<A>::add(a.a(), b.a())); }
  static Tagged<A, TagType> subtract(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) { return Tag<TagType>(Num<A>::subtract(a.a(), b.a())); }
  static Tagged<A, TagType> multiply(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) { return Tag<TagType>(Num<A>::multiply(a.a(), b.a())); }
  static Tagged<A, TagType> divide(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) { return Tag<TagType>(Num<A>::divide(a.a(), b.a())); }
  static Tagged<A, TagType> mod(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) { return Tag<TagType>(Num<A>::mod(a.a(), b.a())); }
};

#endif // FPCPP_CORE_TYPECLASSES_NUM_H