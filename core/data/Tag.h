#ifndef FPCPP_CORE_DATA_TAG_H
#define FPCPP_CORE_DATA_TAG_H

#include <type_traits>
#include <core/data/Concepts.h>

/** @brief Base of all 'Tag's. */
struct TagBase {};

/** @brief Concept which determines if 'A' is a 'Tag'. */
template<typename A>
concept HasTag = std::is_base_of_v<TagBase, A>;

/**
 * @brief A mechanism for tagging values with an erased type.
 *
 * Useful when we need to have a couple of instances of typeclass for a single type.
 *
 * For example, for a given 'Semigroup' typeclass (which provides functionality for combining two values)
 * it does not make sense to have a 'bool' instance of this typeclass, because it would be ambiguous (should we
 * combine with '&&' or '||' ?).
 *
 * Instead, what we can do is to tag 'bool' with either 'AndTag' or 'OrTag', which would result in either
 * of the 'Semigroup' typeclass like so:
 * - 'Semigroup<Tagged<bool, AndTag>>'
 * - 'Semigroup<Tagged<bool, OrTag>>'
 *
 * With the given 'Semigroup' instances, now we can appropriately implement 'bool' semigroup instances like so:
 * - 'Semigroup<Tagged<bool, AndTag>>' with && operator.
 * - 'Semigroup<Tagged<bool, OrTag>>' with || operator.
 */
template<typename A, HasTag TagType_>
struct Tagged {
  using WrappedType = A;
  using TagType = TagType_;

  Tagged() : _a() {}
  explicit Tagged(const A &a) : _a(a) { }

  // ReSharper disable once CppNonExplicitConversionOperator
  operator A() const { return _a; }
  A a() const { return _a; }
private:
  A _a;
};

/** @brief Tags a value. */
template<HasTag TagType, typename A>
Tagged<A, TagType> Tag(const A& a) { return Tagged<A, TagType>{a}; }

/**
 * @brief Determines whether a 'Container' allows tagging its values.
 *
 * Looks inside a 'Container' if it has these methods:
 * - 'Container'.tag<Tag>() -> 'TaggedContainer'
 * - 'TaggedContainer'.unTag() -> 'Container'
 *
 * @tparam Container a container which wraps a value(e.g. Option<A>).
 * @tparam TaggedContainer - a container which wraps a 'Tagged<A, TagType>'(e.g. Option<Tagged<A, TagType>>).
 */
template<
  typename Container,
  typename A = typename Container::ValueType,
  typename TaggedContainer = typename Container::template NewType<Tagged<A, TagBase>>,
  typename Tagged = typename TaggedContainer::ValueType,
  typename InnerA = typename Tagged::WrappedType
>
concept HasContainerTagging = requires(const Container& container, const TaggedContainer& taggedContainer) {
  { container.template tag<TagBase>() } -> std::same_as<TaggedContainer>;
  { taggedContainer.unTag() } -> std::same_as<Container>;
};

/** @brief Determines whether 'A' is tagged. */
template<typename A>
concept IsATagged = std::same_as<A, Tagged<typename A::WrappedType, typename A::TagType>>;

/** @brief Executes 'f' on a tagged values of 'Container'. */
template<
  HasTag TagType,
  HasContainerTagging Container,
  typename A = typename Container::ValueType,
  typename TaggedContainer = typename Container::template NewType<Tagged<A, TagType>>,
  Func2<TaggedContainer, TaggedContainer, TaggedContainer> Func
>
Container PerformContainerTag(const Container& a, const Container& b, Func&& f)  {
  return f(a.template tag<TagType>(), b.template tag<TagType>()).unTag();
}

/** @brief Executes 'f' on a tagged values of 'A'. */
template<
  HasTag TagType,
  typename A,
  typename TaggedA = Tagged<A, TagType>,
  Func2<TaggedA, TaggedA, TaggedA> Func
>
A PerformTag(const A& a, const A& b, Func&& f)  {
  return f(Tag<TagType>(a), Tag<TagType>(b)).a();
}

struct AndTag : TagBase {};
struct OrTag : TagBase {};

#endif //FPCPP_CORE_DATA_TAG_H
