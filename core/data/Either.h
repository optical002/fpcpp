#ifndef FPCPP_CORE_DATA_EITHER_H
#define FPCPP_CORE_DATA_EITHER_H

#include <concepts>
#include <utility>
#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Unit.h>
#include <core/data/Variant.h>

template<typename A>
class Option;

/**
 * @brief It may be either 'Left' or 'Right' value.
 *
 * It is a right based 'Either'.
 *
 * @note In most cases it is used to pass error information on left side.
 */
template<typename L, typename R>
class Either {
public:
  template<typename NewRight>
  using NewType = Either<L, NewRight>;
  using ValueType = R;
  using RightType = R;
  using LeftType = L;
  
  static Either left(const L& value) { return Either(value); }
  static Either right(const R& value) { return Either(value, {}); }

  bool isLeft() const { return _impl.isValueAtIdx(0);; }
  bool isRight() const { return !isLeft(); }

  Option<L> left() const;
  Option<R> right() const;

  template<typename OnLeftFunc, typename OnRightFunc>
  requires std::invocable<OnLeftFunc, L> && std::invocable<OnRightFunc, R>
  void voidFold(OnLeftFunc&& onLeft, OnRightFunc&& onRight) const {
    if (isLeft()) {
      onLeft(_unsafeLeft());
    } else {
      onRight(_unsafeRight());
    }
  }

  template<typename OnLeftFunc, typename OnRightFunc>
  requires std::invocable<OnLeftFunc, L> && std::invocable<OnRightFunc, R>
  auto fold(
    OnLeftFunc&& onLeft, OnRightFunc&& onRight
  ) const {	return isLeft() ? onLeft(_unsafeLeft()) : onRight(_unsafeRight()); }

  template<
    typename Func,
    typename NewRight = std::invoke_result_t<Func, R>,
    typename NewEither = Either<L, NewRight>
  > requires std::invocable<Func, R>
  NewEither map(Func&& f) const {
    return isLeft() ? NewEither::left(_unsafeLeft()) : NewEither::right(f(_unsafeRight()));
  }

  template<
    typename Func,
    typename NewLeft = std::invoke_result_t<Func, L>,
    typename NewEither = Either<NewLeft, R> 
  > requires std::invocable<Func, L>
  NewEither mapLeft(Func&& f) const {
    return isLeft() ? NewEither::left(f(_unsafeLeft())) : NewEither::right(_unsafeRight());
  }

  template<
    typename OnLeftFunc, typename OnRightFunc,
    typename NewLeft = std::invoke_result_t<OnLeftFunc, L>,
    typename NewRight = std::invoke_result_t<OnRightFunc, R>,
    typename NewEither = Either<NewLeft, NewRight>
  > requires std::invocable<OnLeftFunc, L> && std::invocable<OnRightFunc, R>
  NewEither mapBoth(OnLeftFunc&& onLeft, OnRightFunc&& onRight) const {
    return isLeft() ? NewEither::left(onLeft(_unsafeLeft())) : NewEither::right(onRight(_unsafeRight()));
  }

  template<
    typename Func,
    typename NewEither = std::invoke_result_t<Func, R>
  > requires
    std::invocable<Func, R>
    && std::is_convertible_v<NewEither, Either<L, typename NewEither::RightType>>
  auto flatMap(Func&& f) const -> NewEither {
    return isLeft() ? NewEither::left(_unsafeLeft()) : f(_unsafeRight());
  }

  template<
    typename Func,
    typename NewEither = std::invoke_result_t<Func, L>
  > requires
    std::invocable<Func, L>
    && std::is_convertible_v<NewEither, Either<typename NewEither::LeftType, R>>
  auto flatMapLeft(Func&& f) const -> NewEither {
    return isLeft() ? f(_unsafeLeft()) : NewEither::right(_unsafeRight());
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
  ) const { return isLeft() ? onLeft(_unsafeLeft()) : onRight(_unsafeRight()); }

  template<HasTag TagType>
  Either<L, Tagged<R, TagType>> tag() const {
    return fold(
      [](const L& l) {
        return Either<L, Tagged<R, TagType>>::left(l);
      },
      [](const R& r) {
        return Either<L, Tagged<R, TagType>>::right(Tagged<R, TagType>(r));
      }
    );
  }

  auto unTag() const requires IsATagged<R> {
    using InnerR = typename R::WrappedType;
    return fold(
      [](const L& l) {
        return Either<L, InnerR>::left(l);
      },
      [](const auto& r) {
        return Either<L, InnerR>::right(r);
      }
    );
  }
  
private:
  explicit Either(const L& left) : _impl(Variant<L, R>::template create<0>(left)) {}
  Either(const R& right, Unit _) : _impl(Variant<L, R>::template create<1>(right)) {}

  const L& _unsafeLeft() const { return _impl.template _unsafe_get_ref<0>(); }
  const R& _unsafeRight() const { return _impl.template _unsafe_get_ref<1>(); }

  Variant<L, R> _impl;
};

/** @brief A projection of an Either<L, R> to L */
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

/** @brief A projection of an Either<L, R> to R */
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

/** @brief Helper function for constructing 'LeftProjection' of Either<L, ???>  */
template<typename L>
// ReSharper disable once CppInconsistentNaming
LeftProjection<L> Left(const L& left) { return LeftProjection<L>(left);}

/** @brief Helper function for constructing left side of Either<L, R> */
template<typename R, typename L>
// ReSharper disable once CppInconsistentNaming
Either<L, R> LeftE(const L& left) { return Left(left); }

/** @brief Helper function for constructing 'RightProjection' of Either<???, R>  */
template<typename R>
// ReSharper disable once CppInconsistentNaming
RightProjection<R> Right(const R& right) { return RightProjection<R>(right); }

/** @brief Helper function for constructing right side of Either<L, R> */
template<typename L, typename R>
// ReSharper disable once CppInconsistentNaming
Either<L, R> RightE(const R& right) { return Right(right); }

// ReSharper disable once CppUnusedIncludeDirective
#include "Option.h"  // NOLINT(misc-header-include-cycle)

template <typename L, typename R>
Option<L> Either<L, R>::left() const { return isLeft() ? Some(_unsafeLeft()) : None; }

template <typename L, typename R>
Option<R> Either<L, R>::right() const { return isRight() ? Some(_unsafeRight()) : None; }

template<HasEq L, HasEq R>
struct Eq<Either<L, R>> {
  static bool equal(const Either<L, R>& a, const Either<L, R>& b) {
    return a.fold(
      [&b](const L& alValue) {
        return Equal(b.left(), Some(alValue));
      },
      [&b](const R& arValue) {
        return Equal(b.right(), Some(arValue));
      }
    );
  }
};

template<HasToString L, HasToString R>
struct ToString<Either<L, R>> {
  static std::string toStr(const Either<L, R>& value) {
    return value.fold(
      [](const L& l) {
        return std::format("Left({})", ToStr(l));
      },
      [](const R& r) {
        return std::format("Right({})", ToStr(r));
      }
    );
  }
};

#endif // FPCPP_CORE_DATA_EITHER_H
