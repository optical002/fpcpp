#ifndef FPCPP_CORE_DATA_OPTION_H
#define FPCPP_CORE_DATA_OPTION_H

#include <concepts>
#include <core/data/Unit.h>
#include <core/data/Variant.h>
#include <core/data/Tag.h>
#include <core/typeclasses/Eq.h>
#include <core/typeclasses/Str.h>
#include <core/typeclasses/DebugStr.h>

#define OPTION_FIELD_NAME(NAME) __opt_##NAME

#define GET_OR_RETURN(VAR_NAME, OPT) \
  auto OPTION_FIELD_NAME(VAR_NAME) = OPT; \
  if (OPTION_FIELD_NAME(VAR_NAME).isNone()) { \
    return; \
  } \
  auto VAR_NAME = OPTION_FIELD_NAME(VAR_NAME)._unsafeValue();

#define GET_OR_RETURN_LOG(VAR_NAME, OPT, LOG) \
  auto OPTION_FIELD_NAME(VAR_NAME) = OPT; \
  if (OPTION_FIELD_NAME(VAR_NAME).isNone()) { \
    LOG; \
    return; \
  } \
  auto VAR_NAME = OPTION_FIELD_NAME(VAR_NAME)._unsafeValue();

#define SET_OR_LOG(VAR_NAME, OPT, LOG) \
  auto OPTION_FIELD_NAME(VAR_NAME) = OPT; \
  if (OPTION_FIELD_NAME(VAR_NAME).isNone()) { \
    LOG; \
  } \
  VAR_NAME = OPTION_FIELD_NAME(VAR_NAME)._unsafeValue();

#define GET_OR_RETURN_VALUE(VAR_NAME, VALUE, OPT) \
  auto OPTION_FIELD_NAME(VAR_NAME) = OPT; \
  if (OPTION_FIELD_NAME(VAR_NAME).isNone()) { \
    return VALUE; \
  } \
  auto VAR_NAME = OPTION_FIELD_NAME(VAR_NAME)._unsafeValue();

#define GET_OR_RETURN_VALUE_LOG(VAR_NAME, VALUE, OPT, LOG) \
  auto OPTION_FIELD_NAME(VAR_NAME) = OPT; \
  if (OPTION_FIELD_NAME(VAR_NAME).isNone()) { \
    LOG; \
    return VALUE; \
  } \
  auto VAR_NAME = OPTION_FIELD_NAME(VAR_NAME)._unsafeValue();

#define OPT_PTR(PTR) \
  auto __ptrValue = PTR; \

template<typename L, typename R>
class Either;

/**
 * May contain a value of type A.
 */
template<typename A>
class Option {
public:
  template<typename NewA>
  using NewType = Option<NewA>;

  using ValueType = A;
  
  static Option some(const A value){ return Option(value); }
  static Option none() { return Option(); }
  static Option opt(const bool& condition, const A value){
    return condition ? some(value) : none();
  }

  const A& _unsafeValue() const { return _impl.template _unsafe_get_ref<0>(); }

  bool isSome() const { return _impl.isValueAtIdx(0); }
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

  bool valueOut(A& value) const {
    value = _unsafeValue();
    return isSome();
  }

  A getOr(const A& value) const {
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

  template<HasTag TagType>
  Option<Tagged<A, TagType>> tag() const {
    return fold(
      Option<Tagged<A, TagType>>::none(),
      [](const A& value) {
        return Option<Tagged<A, TagType>>::some(Tag<TagType>(value));
      }
    );
  }

  auto unTag() const requires IsATagged<A> {
    using InnerA = typename A::WrappedType;

    return fold(
      Option<InnerA>::none(),
      [](const auto& value) {
        return Option<InnerA>::some(value);
      }
    );
  }

private:
  Option() : _impl(Variant<A, Unit>::template create<1>(Unit())) {}
  explicit Option(const A& value) : _impl(Variant<A, Unit>::template create<0>(value)) {}

  Variant<A, Unit> _impl;
};

template<typename A>
Option<std::decay_t<A>> Some(A value) {
  return Option<std::decay_t<A>>::some(value);
}

struct NoneType {
  template<typename A>
  operator Option<A>() const {
    return Option<A>::none();
  }

  template<typename A>
  Option<std::decay_t<A>> as() const {
    return Option<std::decay_t<A>>::none();
  }
};

inline constexpr NoneType None{};

template<typename A>
Option<std::decay_t<A>> NoneOf() {
  return Option<std::decay_t<A>>::none();
}

#include <core/data/Either.h>

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

template<HasStr A>
struct Str<Option<A>> {
  static std::string toStr(const Option<A>& value) {
    return value.fold(
      std::string("None"),
      [](const A& v) {
        return std::format("Some({})", ToStr(v));
      }
    );
  }
};

template<HasDebugStr A>
struct DebugStr<Option<A>> {
  static std::string toDebugStr(const Option<A>& value) {
    return value.fold(
      std::string("None"),
      [](const A& v) {
        return std::format("Some({})", ToDebugStr(v));
      }
    );
  }
};

#endif // FPCPP_CORE_DATA_OPTION_H