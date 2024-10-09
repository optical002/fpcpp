#ifndef FPCPP_CORE_DATA_TAG_H
#define FPCPP_CORE_DATA_TAG_H

#include <type_traits>

struct TagBase {};

template<typename TagType>
concept HasTag = std::is_base_of_v<TagBase, TagType>;

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

template<HasTag TagType, typename A>
Tagged<A, TagType> Tag(const A& a) { return Tagged<A, TagType>{a}; }

template<
  typename Container,
  typename A = typename Container::ValueType,
  typename TaggedContainer = typename Container::template NewType<Tagged<A, TagBase>>,
  typename Tagged = typename TaggedContainer::ValueType,
  typename InnerA = typename Tagged::WrappedType
>
concept IsTagableContainer = requires(const Container& container, const TaggedContainer& taggedContainer) {
  { container.template tag<TagBase>() } -> std::same_as<TaggedContainer>;
  { taggedContainer.unTag() } -> std::same_as<Container>;
};

template<typename A>
concept IsATagged = std::same_as<A, Tagged<typename A::WrappedType, typename A::TagType>>;

template<
  HasTag TagType,
  IsTagableContainer Container,
  typename A = typename Container::ValueType,
  typename TaggedContainer = typename Container::template NewType<Tagged<A, TagType>>,
  Func2<TaggedContainer, TaggedContainer, TaggedContainer> Func
>
Container PerformContainerTag(const Container& a, const Container& b, Func&& f)  {
  return f(a.template tag<TagType>(), b.template tag<TagType>()).unTag();
}

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
