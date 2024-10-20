#ifndef FPCPP_CORE_TYPECLASSES_ORD_H
#define FPCPP_CORE_TYPECLASSES_ORD_H

#include <concepts>
#include <core/data/Concepts.h>
#include <core/data/Tag.h>

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

template<HasOrd A, HasTag TagType>
struct Ord<Tagged<A, TagType>> {
  static int compare(const Tagged<A, TagType>& a, const Tagged<A, TagType>& b) {
    return Compare(a.a(), b.a());
  }
};

#endif // FPCPP_CORE_TYPECLASSES_ORD_H