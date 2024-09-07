#pragma once

#include <concepts>

template<typename A>
class Option;

/**
 * It may be either 'Left' or 'Right' value.
 In most cases it is used to pass error information on left side.
 */
template<typename L, typename R>
class Either {
public:
  using ValueType = R;
  using RightType = R;
  using LeftType = L;
  
  static Either left(const L& value) {
    auto e = Either();
    e._left = value;
    e._isLeft = true;
    return e;
  }
  static Either right(const R& value) {
    auto e = Either();
    e._right = value;
    e._isLeft = false;
    return e;
  }

  bool isRight() const { return !_isLeft; }
  bool isLeft() const { return _isLeft; }
  
  Option<L> left() const;
  Option<R> right() const;

  template<typename OnLeftFunc, typename OnRightFunc>
  requires std::invocable<OnLeftFunc, L> && std::invocable<OnRightFunc, R>
  void voidFold(OnLeftFunc&& onLeft, OnRightFunc&& onRight) const {
    if (_isLeft) {
      onLeft(_left);
    } else {
      onRight(_right);
    }
  }

  template<typename OnLeftFunc, typename OnRightFunc>
  requires std::invocable<OnLeftFunc, L> && std::invocable<OnRightFunc, R>
  auto fold(
    OnLeftFunc&& onLeft, OnRightFunc&& onRight
  ) const {	return _isLeft ? onLeft(_left) : onRight(_right); }

  template<
    typename Func,
    typename NewRight = std::invoke_result_t<Func, R>,
    typename NewEither = Either<L, NewRight>
  > requires std::invocable<Func, R>
  NewEither map(Func&& f) const {
    return _isLeft ? NewEither::left(_left) : NewEither::right(f(_right));
  }

  template<
    typename Func,
    typename NewLeft = std::invoke_result_t<Func, L>,
    typename NewEither = Either<NewLeft, R> 
  > requires std::invocable<Func, L>
  NewEither mapLeft(Func&& f) const {
    return _isLeft ? NewEither::left(f(_left)) : NewEither::right(_right);
  }

  template<
    typename OnLeftFunc, typename OnRightFunc,
    typename NewLeft = std::invoke_result_t<OnLeftFunc, L>,
    typename NewRight = std::invoke_result_t<OnRightFunc, R>,
    typename NewEither = Either<NewLeft, NewRight>
  > requires std::invocable<OnLeftFunc, L> && std::invocable<OnRightFunc, R>
  NewEither mapBoth(OnLeftFunc&& onLeft, OnRightFunc&& onRight) const {
    return _isLeft ? NewEither::left(onLeft(_left)) : NewEither::right(onRight(_right));
  }

  template<
    typename Func,
    typename NewEither = std::invoke_result_t<Func, R>
  > requires
    std::invocable<Func, R>
    && std::is_convertible_v<NewEither, Either<L, typename NewEither::RightType>>
  auto flatMap(Func&& f) const -> NewEither {
    return _isLeft ? NewEither::left(_left) : f(_right);
  }

  template<
    typename Func,
    typename NewEither = std::invoke_result_t<Func, L>
  > requires
    std::invocable<Func, L>
    && std::is_convertible_v<NewEither, Either<typename NewEither::LeftType, R>>
  auto flatMapLeft(Func&& f) const -> NewEither {
    return _isLeft ? f(_left) : NewEither::right(_right);
  }

  template<
    typename OnLeftFunc,
    typename OnRightFunc,
    typename LeftEither = std::invoke_result_t<OnLeftFunc, L>,
    typename RightEither = std::invoke_result_t<OnRightFunc, R>
  > requires
    std::invocable<OnLeftFunc, L>
    && std::invocable<OnRightFunc, R>
    && std::is_same_v<LeftEither, RightEither>
  auto flatMapBoth(
    OnLeftFunc&& onLeft, OnRightFunc&& onRight
  ) const { return _isLeft ? onLeft(_left) : onRight(_right); }
  
private:
  Either() : _isLeft(false) {}
	 
  L _left;
  R _right;
  bool _isLeft;
};

template<typename L>
struct LeftProjection {
private:
  L _left;
public:
  explicit LeftProjection(L left) : _left(std::move(left)) {}
  
  template<typename R>
  operator Either<L, R>() const {
    return Either<L, R>::left(_left);
  }

  template<typename R>
  Either<L, R> to() const {
    return Either<L, R>::left(_left);
  }
};

template<typename R>
struct RightProjection {
private:
  R _right;
public:
  explicit RightProjection(R right) : _right(std::move(right)) {}

  template<typename L>
  operator Either<L, R>() const {
    return Either<L, R>::right(_right);
  }

  template<typename L>
  Either<L, R> to() const {
    return Either<L, R>::right(_right);
  }
};

template<typename L>
// ReSharper disable once CppInconsistentNaming
LeftProjection<L> Left(const L& left) { return LeftProjection<L>(left);}

template<typename R, typename L>
// ReSharper disable once CppInconsistentNaming
Either<L, R> LeftE(const L& left) { return Left(left); }

template<typename R>
// ReSharper disable once CppInconsistentNaming
RightProjection<R> Right(const R& right) { return RightProjection<R>(right); }

template<typename L, typename R>
// ReSharper disable once CppInconsistentNaming
Either<L, R> RightE(const R& right) { return Right(right); }

// ReSharper disable once CppUnusedIncludeDirective
#include "Option.h"  // NOLINT(misc-header-include-cycle)

template <typename L, typename R>
Option<L> Either<L, R>::left() const { return _isLeft ? Some(_left) : None; }

template <typename L, typename R>
Option<R> Either<L, R>::right() const { return isRight() ? Some(_right) : None; }
