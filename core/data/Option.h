#ifndef FPCPP_CORE_DATA_OPTION_H
#define FPCPP_CORE_DATA_OPTION_H

#include <concepts>
#include <utility>
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

  bool isSome() const { return _hasValue; }
  bool isNone() const { return !_hasValue; }

  template<std::invocable<A> Func>
  auto map(Func&& f) const {
    return Option<decltype(f(std::declval<A>()))>::opt(_hasValue, f(_unsafeValue));
  }

  template<std::invocable<A> Func>
  auto flatMap(Func&& f) const -> decltype(f(std::declval<A>())) {
    return _hasValue ? f(_unsafeValue) : decltype(f(std::declval<A>()))::none();
  }

  template<std::invocable<A> Func>
  void ifSome(Func&& onSome) const {
    if (_hasValue) {
      onSome(_unsafeValue);
    }
  }

  template<std::invocable OnEmptyFunc, std::invocable<A> OnSomeFunc>
  void voidFold(OnEmptyFunc&& onEmpty, OnSomeFunc&& onSome) const {
    if (_hasValue) {
      onSome(_unsafeValue);
    } else {
      onEmpty();
    }
  }

  template<typename B, std::invocable<A> OnSomeFunc>
  B fold(B onEmpty, OnSomeFunc&& onSome) const {
    return _hasValue ? onSome(_unsafeValue) : onEmpty;
  }

  bool valueOut(A& value) {
    value = _unsafeValue;
    return _hasValue;
  }

  A getOr(const A& value){
    return _hasValue ? _unsafeValue : value;
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
  Option() : _hasValue(false) {}
  explicit Option(A value) : _unsafeValue(std::move(value)), _hasValue(true) {}
	 
  A _unsafeValue;
  bool _hasValue;
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

#include "Either.h"

template <typename A>
template <typename R>
Either<A, R> Option<A>::toLeft(const R& rightValue) const {
  return _hasValue ? LeftE<R>(_unsafeValue) : Right(rightValue);
}

template <typename A>
template <typename L>
Either<L, A> Option<A>::toRight(const L& leftValue) const {
  return _hasValue ? RightE<L>(_unsafeValue) : Left(leftValue);
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