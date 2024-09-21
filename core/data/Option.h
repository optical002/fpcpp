#ifndef FPCPP_CORE_DATA_OPTION_H
#define FPCPP_CORE_DATA_OPTION_H

#include <concepts>
#include <utility>

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

  template<typename Func>
  requires std::invocable<Func, A>
  auto map(Func&& f) const {
    return Option<decltype(f(std::declval<A>()))>::opt(_hasValue, f(_unsafeValue));
  }

  template<typename Func>
  requires std::invocable<Func, A>
  auto flatMap(Func&& f) const -> decltype(f(std::declval<A>())) {
    return _hasValue ? f(_unsafeValue) : decltype(f(std::declval<A>()))::none();
  }

  template<typename Func>
  requires std::invocable<Func, A>
  void ifSome(Func&& onSome) const {
    if (_hasValue) {
      onSome(_unsafeValue);
    }
  }

  template<typename OnEmptyFunc, typename OnSomeFunc>
  requires std::invocable<OnSomeFunc, A> && std::invocable<OnEmptyFunc>
  void voidFold(OnEmptyFunc&& onEmpty, OnSomeFunc&& onSome) const {
    if (_hasValue) {
      onSome(_unsafeValue);
    } else {
      onEmpty();
    }
  }

  template<typename B, typename OnSomeFunc>
  requires std::invocable<OnSomeFunc, A>
  B fold(B onEmpty, OnSomeFunc&& onSome) const {
    return _hasValue ? onSome(_unsafeValue) : onEmpty;
  }

  bool valueOut(A& value) {
    value = _unsafeValue;
    return _hasValue;
  }

  A valueOr(const A& value){
    return _hasValue ? _unsafeValue : value;
  }
  
  template<typename R>
  Either<A, R> toLeft(const R& rightValue);

  template<typename L>
  Either<L, A> toRight(const L& leftValue);

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
Either<A, R> Option<A>::toLeft(const R& rightValue) {
  return _hasValue ? LeftE<R>(_unsafeValue) : Right(rightValue);
}

template <typename A>
template <typename L>
Either<L, A> Option<A>::toRight(const L& leftValue) {
  return _hasValue ? RightE<L>(_unsafeValue) : Left(leftValue);
}

#endif // FPCPP_CORE_DATA_OPTION_H