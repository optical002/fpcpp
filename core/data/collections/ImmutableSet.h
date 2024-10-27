#ifndef FPCPP_CORE_DATA_IMMUTABLE_SET_H
#define FPCPP_CORE_DATA_IMMUTABLE_SET_H

#include <set>
#include <core/data/Option.h>

/** @brief An immutable generic collection with unique values. */
template<typename A>
struct ImmutableSet {
public:
  template<typename NewValue>
  using NewType = ImmutableSet<NewValue>;
  using ValueType = A;

  explicit ImmutableSet(const std::set<A>& data) : _data(data) {}

  [[nodiscard]] std::size_t size() const { return _data.size(); }

  [[nodiscard]] bool isEmpty() const { return size() == 0; }

  typename std::set<A>::const_iterator begin() const {
    return _data.cbegin();
  }
  typename std::set<A>::const_iterator end() const {
    return _data.cend();
  }

  // TODO implement operations:
  // - map: (f: A -> B) : Set[B]
  // - flatMap: (f: A -> Set[B]) : Set[B]
  // - flatten: [Set[A]] -> Set[A]
  // - reduce: (f: A -> B) : B
  // - fold: (initial: B, f: A -> B) : B
  // - filter: (f: A -> bool) : Set[A]
  // - find: (f: A -> bool) : Option[A]
  // - groupBy: (f: A -> K) : Map[K, Set[A]]
  // - collect: (f: A -> Option[A]) : Set[A]
  // - exists: (f: A -> bool) : bool
  // - forAll: (f: A -> bool) : bool
  // - mkString: (separator: String) : String
  // - mkString: (f: A -> String) : String
  // - union: (other: Set[A]) : Set[A]
  // - intersect: (other: Set[A]) : Set[A]
  // - diff: (other: Set[A]) : Set[A]

  // TODO implement conversions:
  // - toMap: ImmutableArray[std::pair<A, B>] -> ImmutableMap[A, B]
  // - toMap: (f: A -> (K, V)) -> ImmutableMap[K, V]
  // - toVector: ImmutableVector[A]

  // TODO implement typeclasses:
  // - Eq
  // - Default
  // - Semigroup
  // - Str
  // - DebugStr

  // TODO add tag and untag

private:
  std::set<A> _data;
};

template<
  typename... Values,
  typename A = std::decay_t<std::tuple_element_t<0, std::tuple<Values...>>>
> requires AllSame<Values...>
ImmutableSet<A> ImmSet(Values... values) {
  return ImmutableSet<A>({values...});
}

template<typename A>
ImmutableSet<A> EmptyImmSet() { return ImmutableSet<A>({}); }


#endif //FPCPP_CORE_DATA_IMMUTABLE_SET_H
