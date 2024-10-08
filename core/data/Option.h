#ifndef FPCPP_CORE_DATA_OPTION_H
#define FPCPP_CORE_DATA_OPTION_H

#include <concepts>
#include <utility>
#include <variant>
#include <core/data/Unit.h>
#include <core/typeclasses/Eq.h>
#include <core/typeclasses/ToString.h>

template<typename L, typename R>
class Either;

/**
 * May contain a value of type A.
 */
template<typename A>
class Option {
public:
  using ValueType = A;
  
  static Option some(const A& value){ return Option(value); }
  static Option none() { return Option(); }
  static Option opt(const bool& condition, const A& value){
    return condition ? some(value) : none();
  }

  bool isSome() const { return std::holds_alternative<A>(_impl); }
  bool isNone() const { return !isSome(); }

  template<
    std::invocable<A> Func,
    typename B = std::invoke_result_t<Func, A>
  >
  Option<B> map(Func&& f) const {
    return isSome() ? Option<B>::some(f(_unsafeValue())) : Option<B>::none();
  }

  template<
    std::invocable<A> Func,
    typename B = typename std::invoke_result_t<Func, A>::ValueType
  >
  Option<B> flatMap(Func&& f) const {
    return isSome() ? f(_unsafeValue()) : Option<B>::none();
  }

  template<std::invocable<A> Func>
  void ifSome(Func&& onSome) const {
    if (isSome()) {
      onSome(_unsafeValue());
    }
  }

  template<std::invocable OnEmptyFunc, std::invocable<A> OnSomeFunc>
  void voidFold(OnEmptyFunc&& onEmpty, OnSomeFunc&& onSome) const {
    if (isSome()) {
      onSome(_unsafeValue());
    } else {
      onEmpty();
    }
  }

  template<typename B, std::invocable<A> OnSomeFunc>
  B fold(B onEmpty, OnSomeFunc&& onSome) const {
    return isSome() ? onSome(_unsafeValue()) : onEmpty;
  }

  bool valueOut(A& value) {
    value = _unsafeValue();
    return isSome();
  }

  A getOr(const A& value){
    return isSome() ? _unsafeValue() : value;
  }
  
  template<typename R>
  Either<A, R> toLeft(const R& rightValue) const;

  template<typename L>
  Either<L, A> toRight(const L& leftValue) const;

  auto flatten() const requires HasValueType<A> {
    using InnerA = typename A::ValueType;
    return fold(
      Option<InnerA>::none(),
      [](const A& innerOption) {
        return innerOption.fold(
          Option<InnerA>::none(),
          [](const InnerA& innerValue) {
            return Option<InnerA>::some(innerValue);
          }
        );
      }
    );
  }

private:
  Option() : _impl(Unit()) {}
  explicit Option(const A& value) : _impl(value) {}

  const A& _unsafeValue() const { return std::get<A>(_impl); }

  std::variant<A, Unit> _impl;
};

template<typename A>
// ReSharper disable once CppInconsistentNaming
Option<A> Some(const A& value) {
  return Option<A>::some(value);
}

struct NoneType {
  template<typename A>
  operator Option<A>() const {
    return Option<A>::none();
  }

  template<typename A>
  Option<A> as() const {
    return Option<A>::none();
  }
};

// ReSharper disable once CppInconsistentNaming
inline constexpr NoneType None{};

template<typename A>
Option<A> NoneOf() {
  return Option<A>::none();
}

#include "Either.h"

template <typename A>
template <typename R>
Either<A, R> Option<A>::toLeft(const R& rightValue) const {
  return isSome() ? LeftE<R>(_unsafeValue()) : Right(rightValue);
}

template <typename A>
template <typename L>
Either<L, A> Option<A>::toRight(const L& leftValue) const {
  return isSome() ? RightE<L>(_unsafeValue()) : Left(leftValue);
}

template<HasEq A>
struct Eq<Option<A>> {
  static bool equal(const Option<A>& a, const Option<A>& b) {
    return (
        a.isNone() && b.isNone()
      ) || (
        a.fold(
          false,
          [&b](const A& aValue) {
            return b.fold(
              false,
              [&aValue](const A& bValue) {
                return Equal(aValue, bValue);
              }
            );
          }
        )
      );
  }
};

template<HasToString A>
struct ToString<Option<A>> {
  static std::string toStr(const Option<A>& value) {
    return value.fold(
      std::string("None"),
      [](const A& v) {
        return std::format("Some({})", ToStr(v));
      }
    );
  }
};

#endif // FPCPP_CORE_DATA_OPTION_H