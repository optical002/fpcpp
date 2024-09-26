#ifndef FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H
#define FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H

#include <array>

template<typename A, std::size_t N>
class ImmutableArray {
public:
  explicit ImmutableArray(const std::array<A, N>& arr) : _data(arr) {}

  A& operator[](std::size_t) = delete;
  A& at(std::size_t) = delete;

  const A& operator[](std::size_t index) const {
    return _data[index];
  }
  const A& at(std::size_t index) const {
    return _data.at(index);
  }

  [[nodiscard]] std::size_t size() const { return N; }

  typename std::array<A, N>::const_iterator begin() const {
    return _data.cbegin();
  }
  typename std::array<A, N>::const_iterator end() const {
    return _data.cend();
  }

  // TODO
  // - map
  // - flatMap
  // - reduce // no initial state
  // - fold // has initial state
  // - scan
  // - sortBy A -> Ord, (Ascending/Descending)
  // - filter A -> bool
  // - headOpt
  // - find A -> bool
  // - exists A -> bool
  // - forall A -> bool
  // - groupBy Groups elements by a key function. def groupBy[K](f: A => K): Map[K, List[A]]
      //  def groupBy[K](f: A => K): Map[K, List[A]] = {
      //   val map = scala.collection.mutable.Map[K, List[A]]()
      //
      //   for (element <- this) {
      //     val key = f(element) // Apply the key function
      //     // Append the element to the corresponding list in the map
      //     map(key) = map.getOrElse(key, List()) :+ element
      //   }
      //
      //   map.toMap // Convert mutable map to immutable map
      // }
  // - collect A -> Option<B>
  // - distinct
  // - take/drop n
  // - take/drop while A -> predicate
  // - contains
  // - isEmpty
  // - reverse
  // - mkString
  // - sum
  // - zip (zips from left and right, until either one finished)
  // - zipAll (left, right, default)
  // - zipWithIndex
  // - slice (idxFrom, idxTo)

private:
  std::array<A, N> _data;
};

// TODO construction
// - from variadic parameters.

// TODO typeclasses
// - Default
// - Eq
// - Semigroup
// - ToString

#endif //FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H
