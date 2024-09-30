#ifndef FPCPP_CORE_DATA_CHANGES_H
#define FPCPP_CORE_DATA_CHANGES_H

#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Eq.h>

template<typename A>
struct Changes {
  Changes(const A& previous, const A& next) : _previous(previous), _next(next) {}

  A previous() const { return _previous; }
  A next() const { return _next; }
private:
  A _previous, _next;
};

template<HasToString A>
struct ToString<Changes<A>> {
  static std::string toStr(const Changes<A>& a) {
    return std::format("Changes(previous={}, next={})", ToStr(a.previous()), ToStr(a.next()));
  }
};

template<HasEq A>
struct Eq<Changes<A>> {
  static bool eq(const Changes<A>& a, const Changes<A>& b) {
    return Equal(a.previous(), b.previous()) && Equal(a.next(), b.next());
  }
};

#endif // FPCPP_CORE_DATA_CHANGES_H