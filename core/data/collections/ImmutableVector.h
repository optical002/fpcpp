#ifndef FPCPP_CORE_DATA_IMMUTABLE_VECTOR_H
#define FPCPP_CORE_DATA_IMMUTABLE_VECTOR_H

#include <vector>
#include <core/data/Option.h>

/** @brief An immutable generic collection which has dynamic length. */
template<typename A>
struct ImmutableVector {
public:
  template<typename NewValue>
  using NewType = ImmutableVector<NewValue>;
  using ValueType = A;

  explicit ImmutableVector(const std::vector<A>& data) : _data(data) {}

  [[nodiscard]] std::size_t size() const { return _data.size(); }

  [[nodiscard]] bool isEmpty() const { return size() == 0; }

  A _unsafe_at(std::size_t index) const {
    return _data.at(index);
  }
  Option<A> at(std::size_t index) const {
    return index < size() ? Some(_data.at(index)) : None;
  }
  Option<A> operator[](const std::size_t index) const {
    return at(index);
  }

  typename std::vector<A>::const_iterator begin() const {
    return _data.cbegin();
  }
  typename std::vector<A>::const_iterator end() const {
    return _data.cend();
  }

  // TODO implement operations:
  // - map: (f: A -> B) : Vector[B]
  // - flatMap: (f: A -> Vector[B]) : Vector[B]
  // - flatten: [Vector[A]] -> Vector[A]
  // - reduce: (f: A -> B) : B
  // - fold: (initial: B, f: A -> B) : B
  // - orderBy: (f: A -> Ord[?], order: OrderKind) : Vector[A]
  // - filter: (f: A -> bool) : Vector[A]
  // - headOpt: Option[A]
  // - tailOpt: Option[A]
  // - find: (f: A -> bool) : Option[A]
  // - groupBy: (f: A -> K) : Map[K, Vector[A]]
  // - collect: (f: A -> Option[A]) : Vector[A]
  // - take: (n: Int) : Vector[A]
  // - drop: (n: Int) : Vector[A]
  // - takeWhile: (f: A -> bool) : Vector[A]
  // - dropWhile: (f: A -> bool) : Vector[A]
  // - slice: (idxFrom: Int, idxTo: Int) : Vector[A]
  // - reverse: Vector[A]
  // - exists: (f: A -> bool) : bool
  // - forAll: (f: A -> bool) : bool
  // - mkString: (separator: String) : String
  // - mkString: (f: A -> String) : String

  // TODO implement conversions:
  // - toMap: ImmutableArray[std::pair<A, B>] -> ImmutableMap[A, B]
  // - toMap: (f: A -> (K, V)) -> ImmutableMap[K, V]
  // - toSet: ImmutableSet[A]

  // TODO implement typeclasses:
  // - Eq
  // - Default
  // - Semigroup
  // - Str
  // - DebugStr

  // TODO add tag and untag

private:
  std::vector<A> _data;
};

template<
  typename... Values,
  typename A = std::decay_t<std::tuple_element_t<0, std::tuple<Values...>>>
> requires AllSame<Values...>
ImmutableVector<A> ImmVector(Values... values) {
  return ImmutableVector<A>({values...});
}

template<typename A>
ImmutableVector<A> EmptyImmVector() { return ImmutableVector<A>({}); }

#endif //FPCPP_CORE_DATA_IMMUTABLE_VECTOR_H
