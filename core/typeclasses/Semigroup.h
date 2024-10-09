#ifndef FPCPP_CORE_TYPECLASSES_SEMIGROUP_H
#define FPCPP_CORE_TYPECLASSES_SEMIGROUP_H

#include <concepts>
#include <string>
#include <core/data/Concepts.h>
#include <core/data/Tag.h>

template<typename A>
struct Semigroup;

template<typename A>
concept HasSemigroup = requires(const A& a, const A& b) {
  { Semigroup<A>::combine(a, b) } -> std::same_as<A>;
};

template<HasSemigroup A>
A Combine(const A& a, const A& b) {
  return Semigroup<A>::combine(a, b);
}

template<HasTag TagType, IsTagableContainer Container>
Container CombineTag(const Container& a, const Container& b)  {
  return PerformContainerTag<TagType>(a, b, [](auto a, auto b) {return Combine(a, b);});
}

template<HasTag TagType, typename A>
A CombineTag(const A& a, const A& b)  {
  return PerformTag<TagType>(a, b, [](auto a, auto b) {return Combine(a, b);});
}

template<>
struct Semigroup<std::string> {
  static std::string combine(const std::string& a, const std::string& b) {
    return a + b;
  }
};

template<IsArithmetic A>
struct Semigroup<A> {
  static A combine(const A& a, const A& b) { return a + b; }
};

template<>
struct Semigroup<Tagged<bool, AndTag>> {
  static Tagged<bool, AndTag> combine(
    const Tagged<bool, AndTag>& a, const Tagged<bool, AndTag>& b
  ) {
    return Tag<AndTag>(a && b);
  }
};

template<>
struct Semigroup<Tagged<bool, OrTag>> {
  static Tagged<bool, OrTag> combine(
    const Tagged<bool, OrTag>& a, const Tagged<bool, OrTag>& b
  ) {
    return Tag<OrTag>(a || b);
  }
};

template<HasSemigroup A, HasTag TagType>
struct Semigroup<Tagged<A, TagType>> {
  static Tagged<A, TagType> combine(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) {
    return Tag<TagType>(Combine(a.a(), b.a()));
  }
};

#endif // FPCPP_CORE_TYPECLASSES_SEMIGROUP_H