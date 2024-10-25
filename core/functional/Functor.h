#ifndef FPCPP_CORE_TYPECLASSES_FUNCTOR_H
#define FPCPP_CORE_TYPECLASSES_FUNCTOR_H

#include <concepts>
#include <functional>
#include <utility>

namespace Functor {
  template<
    typename FA,
    typename A = typename FA::ValueType,
    std::invocable<A> Func,
    typename B = std::invoke_result_t<Func, A>,
    typename FB = typename FA::template NewType<B>
  >
  FB Map(const FA& fa, Func&& f) {
    return fa.map(std::forward<Func>(f));
  }
}

#endif //FPCPP_CORE_TYPECLASSES_FUNCTOR_H
