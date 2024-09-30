#ifndef FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H
#define FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H

#include <array>
#include <core/data/Concepts.h>
#include <core/data/Option.h>
#include <core/typeclasses/Eq.h>
#include <core/typeclasses/Ord.h>
#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Default.h>

#include "Enums.h"

template<typename A, std::size_t N>
class ImmutableArray {
public:
  using ValueType = A;
  static constexpr std::size_t N_ = N;

  explicit ImmutableArray(const std::array<A, N>& arr) : _data(arr) {}

  A& operator[](std::size_t) = delete;
  A& at(std::size_t) = delete;

  const A& operator[](std::size_t index) const {
    return _data[index];
  }
  const A& at(std::size_t index) const {
    return _data.at(index);
  }
  template<std::size_t Index>
  const A& at() const requires (Index < N) {
    return _data[Index];
  }

  [[nodiscard]] static std::size_t size() { return N; }

  [[nodiscard]] static bool isEmpty() { return size() == 0; }

  typename std::array<A, N>::const_iterator begin() const {
    return _data.cbegin();
  }
  typename std::array<A, N>::const_iterator end() const {
    return _data.cend();
  }

  template<std::invocable<A> Map, typename B = std::invoke_result_t<Map, A>>
  ImmutableArray<B, N> map(Map&& map) const {
    auto newArray = std::array<B, N>{};
    for (std::size_t i = 0; i < N; ++i) {
      newArray[i] = map(at(i));
    }
    return ImmutableArray<B, N>(newArray);
  }

  template<
    std::invocable<A> Func,
    typename ImmArrayB = std::invoke_result_t<Func, A>,
    std::size_t ImmArrayBSize = ImmArrayB::N_,
    std::size_t NewN = N * ImmArrayBSize,
    typename B = ImmArrayB::ValueType
  >
  ImmutableArray<B, NewN> flatMap(Func&& f) const {
    auto newArray = std::array<B, NewN>{};

    std::size_t index = 0;
    for (const auto& elem : _data) {
      auto mappedArray = f(elem);
      std::copy(mappedArray.begin(), mappedArray.end(), newArray.begin() + index);
      index += ImmArrayBSize;
    }

    return ImmutableArray<B, NewN>(newArray);
  }

  auto flatten() const requires HasValueType<A> {
    using InnerA = typename A::ValueType;
    constexpr std::size_t InnerN = A::N_;
    constexpr std::size_t NewN = N * InnerN;

    std::array<InnerA, NewN> result;
    std::size_t index = 0;

    for (std::size_t i = 0; i < N; ++i) {
      const auto& innerArray = _data[i];
      for (std::size_t j = 0; j < InnerN; ++j) {
        result[index++] = innerArray[j];
      }
    }

    return ImmutableArray<InnerA, NewN>(result);
  }

  template<
    typename Func,
    IsConvertibleFrom<A> B = std::invoke_result_t<Func, A, A>
  > requires std::invocable<Func, B, A>
  B reduce(Func&& f) const {
    B result = _data[0];
    for (std::size_t i = 1; i < N; ++i) {
      result = f(result, _data[i]);
    }
    return result;
  }

  template<typename B, std::invocable<B, A> Func>
  requires std::same_as<std::invoke_result_t<Func, B, A>, B>
  B fold(B initial, Func&& f) const {
    B result = initial;
    for (std::size_t i = 0; i < N; ++i) {
      result = f(result, _data[i]);
    }
    return result;
  }

  ImmutableArray order(Order order_ = Order::Ascending) const requires HasOrd<A> {
    std::array<A, N> copy;
    std::copy(std::begin(_data), std::end(_data), copy.begin());

    std::sort(copy.begin(), copy.end(), [order_](auto& a, auto& b) {
      return order_ == Order::Ascending ? Compare(a, b) < 0 : Compare(a, b) > 0;
    });

    return ImmutableArray(copy);
  }

  template<std::invocable<A> Func> requires HasOrd<std::invoke_result_t<Func, A>>
  ImmutableArray orderBy(Func&& f, Order order_ = Order::Ascending) const {
    std::array<A, N> copy;
    std::copy(std::begin(_data), std::end(_data), copy.begin());

    std::sort(copy.begin(), copy.end(), [
      f = std::forward<Func>(f), order_
    ](auto& a, auto& b) {
      return order_ == Order::Ascending ? Compare(f(a), f(b)) < 0 : Compare(f(a), f(b)) > 0;
    });

    return ImmutableArray(copy);
  }

  A head() const requires (N > 0) { return _data[0]; }
  A tail() const requires (N > 0) { return _data[size() - 1]; }

  template<Predicate<A> Func>
  Option<A> find(Func&& f) const {
    for (const auto& element : _data) {
      if (f(element)) {
        return Some(element);
      }
    }
    return None;
  }

  template<Predicate<A> Func>
  bool exists(Func&& f) const {
    for (const auto& element : _data) {
      if (f(element)) {
        return true;
      }
    }
    return false;
  }

  template<Predicate<A> Func>
  bool forall(Func&& f) const {
    for (const auto& element : _data) {
      if (!f(element)) {
        return false;
      }
    }
    return true;
  }

  // TODO implement 'groupBy' after ImmutableMap is implemented.


  // TODO
  // v- map
  // x- flatMap
  // v- flatten
  // v- reduce // no initial state
  // v- fold // has initial state
  // v- orderBy A -> Ord, (Ascending/Descending)
  // x- filter A -> bool
  // v- headOpt
  // v- tailOpt
  // v- find A -> bool
  // v- exists A -> bool
  // v- forall A -> bool
  // v- groupBy Groups elements by a key function. def groupBy[K](f: A => K): Map[K, List[A]]
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
  // x- collect A -> Option<B>
  // x- distinct
  // TODO Continue from here
  // v- take/drop n
  // v- take/drop while A -> predicate
  // v- contains
  // v- isEmpty
  // v- reverse
  // v- mkString
  // v- sum
  // v- zip (zips from left and right, until either one finished)
  // v- zipAll (left, right, default)
  // v- zipWithIndex
  // v- slice (idxFrom, idxTo)

private:
  std::array<A, N> _data;
};

template<
  typename... Values,
  std::size_t N = sizeof...(Values),
  typename A = std::decay_t<std::tuple_element_t<0, std::tuple<Values...>>>
> requires AllSame<Values...>
ImmutableArray<A, N> ImmArray(Values... values) {
  return ImmutableArray<A, N>({values...});
}

template<typename A>
ImmutableArray<A, 0> EmptyImmArray() { return ImmutableArray<A, 0>({}); }

template<HasEq A, std::size_t N>
struct Eq<ImmutableArray<A, N>> {
  static bool equal(const ImmutableArray<A, N>& a, const ImmutableArray<A, N>& b) {
    if (a.isEmpty() && b.isEmpty()) return true;

    for (std::size_t i = 0; i < N; i++) {
      if (!Equal(a[i], b[i])) {
        return false;
      }
    }

    return true;
  }
};

// TODO typeclasses
// - Default
// - Eq
// - Semigroup
// - ToString

#endif //FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H
