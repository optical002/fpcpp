#ifndef FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H
#define FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H

#include <array>
#include <sstream>
#include <core/data/Concepts.h>
#include <core/data/Option.h>
#include <core/data/Enums.h>
#include <core/data/Functions.h>
#include <core/data/Tag.h>
#include <core/typeclasses/Eq.h>
#include <core/typeclasses/Ord.h>
#include <core/typeclasses/Str.h>
#include <core/typeclasses/DebugStr.h>
#include <core/typeclasses/Default.h>
#include <core/typeclasses/Num.h>
#include <core/typeclasses/Semigroup.h>

/** @brief An immutable generic collection which has fixed length. */
template<typename A, std::size_t N>
struct ImmutableArray {
public:
  template<typename NewA>
  using NewType = ImmutableArray<NewA, N>;

  using ValueType = A;
  static constexpr std::size_t N_ = N;

  explicit ImmutableArray(const std::array<A, N> arr) : _data(arr) {}

  A _unsafe_at(std::size_t index) const {
    return _data.at(index);
  }
  Option<A> at(std::size_t index) const {
    return index < N ? Some(_data[index]) : None;
  }
  Option<A> operator[](const std::size_t index) const {
    return at(index);
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
      newArray[i] = map(_unsafe_at(i));
    }
    return ImmutableArray<B, N>(newArray);
  }

  template<
    std::invocable<A> Func,
    typename ImmArrayB = std::invoke_result_t<Func, A>,
    std::size_t ImmArrayBSize = ImmArrayB::N_,
    std::size_t NewN = N * ImmArrayBSize,
    typename B = typename ImmArrayB::ValueType
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
        result[index++] = innerArray._unsafe_at(j);
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

  template<
    std::size_t ToTake,
    std::size_t NewN = std::min(ToTake, N)
  >
  ImmutableArray<A, NewN> take() const {
    std::array<A, NewN> newArray;
    std::copy(_data.begin(), _data.begin() + NewN, newArray.begin());
    return ImmutableArray<A, NewN>(newArray);
  }

  template<
    std::size_t ToDrop,
    std::size_t NewN = std::max(static_cast<std::size_t>(0), N - ToDrop)
  >
  ImmutableArray<A, NewN> drop() const {
    std::array<A, NewN> newArray;
    std::copy(_data.begin() + ToDrop, _data.end(), newArray.begin());
    return ImmutableArray<A, NewN>(newArray);
  }

  ImmutableArray reverse() const {
    std::array<A, N> newArray;
    std::reverse_copy(_data.begin(), _data.end(), newArray.begin());
    return ImmutableArray(newArray);
  }

  std::string mkStringToStr(std::string separator = ", ") const requires HasStr<A> {
    std::ostringstream oss;

    for (std::size_t i = 0; i < N; ++i) {
      oss << ToStr(_data[i]);
      if (i < N - 1) {
        oss << separator;
      }
    }

    return oss.str();
  }

  template<Func1<A, std::string> Func>
  std::string mkString(Func&& f, std::string separator = ", ") const {
    std::ostringstream oss;

    for (std::size_t i = 0; i < N; ++i) {
      oss << f(_data[i]);
      if (i < N - 1) {
        oss << separator;
      }
    }

    return oss.str();
  }

  A sum() const requires HasNum<A> {
    A result = _data[0];
    for (std::size_t i = 1; i < N; ++i) {
      result = Num<A>::add(result, _data[i]);
    }
    return result;
  }

  A combine() const requires HasSemigroup<A> {
    A result = _data[0];
    for (std::size_t i = 1; i < N; ++i) {
      result = Combine(result, _data[i]);
    }
    return result;
  }

  std::string combine() const requires std::same_as<A, const char*>{
    auto result = std::string(_data[0]);
    for (std::size_t i = 1; i < N; ++i) {
      result = Combine(result, std::string(_data[i]));
    }
    return result;
  }

  template<
    typename B,
    std::invocable<A, B> Zipper,
    typename Result = std::invoke_result_t<Zipper, A, B>,
    std::size_t BN,
    std::size_t NewN = std::min(N, BN)
  >
  ImmutableArray<Result, NewN> zipWith(ImmutableArray<B, BN> other, Zipper&& zipper) const {
    std::array<Result, NewN> result;
    for (std::size_t i = 0; i < NewN; ++i) {
      result[i] = zipper(_data[i], other._unsafe_at(i));
    }
    return ImmutableArray<Result, NewN>(result);
  }

  template<
    typename B,
    typename Result = std::tuple<A, B>,
    std::size_t BN,
    std::size_t NewN = std::min(N, BN)
  >
  ImmutableArray<Result, NewN> zipWith(ImmutableArray<B, BN> other) const {
    std::array<Result, NewN> result;
    for (std::size_t i = 0; i < NewN; ++i) {
      result[i] = Tpl(_data[i], other._unsafe_at(i));
    }
    return ImmutableArray<Result, NewN>(result);
  }

  template<
    typename B,
    std::invocable<A, B> Zipper,
    typename Result = std::invoke_result_t<Zipper, A, B>,
    std::size_t BN,
    std::size_t NewN = std::max(N, BN)
  >
  ImmutableArray<Result, NewN> zipWithAll(
    ImmutableArray<B, BN> other, Zipper&& zipper, const B& default_
  ) const {
    std::array<Result, NewN> result;
    for (std::size_t i = 0; i < NewN; ++i) {
      A first = i < N ? _data[i] : default_;
      B second = i < BN ? other._unsafe_at(i) : default_;
      result[i] = zipper(first, second);
    }
    return ImmutableArray<Result, NewN>(result);
  }

  template<
    typename B,
    typename Result = std::tuple<A, B>,
    std::size_t BN,
    std::size_t NewN = std::max(N, BN)
  >
  ImmutableArray<Result, NewN> zipWithAll(
    ImmutableArray<B, BN> other, const B& default_
  ) const {
    std::array<Result, NewN> result;
    for (std::size_t i = 0; i < NewN; ++i) {
      A first = i < N ? _data[i] : default_;
      B second = i < BN ? other._unsafe_at(i) : default_;
      result[i] = Tpl(first, second);
    }
    return ImmutableArray<Result, NewN>(result);
  }

  ImmutableArray<std::tuple<A, unsigned int>, N> zipWithIndex() const {
    std::array<std::tuple<A, unsigned int>, N> result;
    for (std::size_t i = 0; i < N; ++i) {
      result[i] = Tpl(_data[i], i);
    }
    return ImmutableArray<std::tuple<A, unsigned int>, N>(result);
  }

  template<
    std::size_t idxFrom, std::size_t idxTo,
    std::size_t NewN = idxTo - idxFrom + 1
  > requires (idxFrom <= idxTo && idxTo <= N)
  ImmutableArray<A, NewN> slice() const {
    std::array<A, NewN> result;
    for (std::size_t i = 0; i < NewN; ++i) {
      result[i] = _data[idxFrom + i];
    }
    return ImmutableArray<A, NewN>(result);
  }

  template<HasTag TagType>
  ImmutableArray<Tagged<A, TagType>, N> tag() const {
    std::array<Tagged<A, TagType>, N> result;
    for (std::size_t i = 0; i < N; ++i) {
      result[i] = Tag<TagType>(_data[i]);
    }
    return ImmutableArray<Tagged<A, TagType>, N>(result);
  }

  auto unTag() const requires IsATagged<A> {
    using InnerA = typename A::WrappedType;

    std::array<InnerA, N> result;
    for (std::size_t i = 0; i < N; ++i) {
      result[i] = _data[i];
    }
    return ImmutableArray<InnerA, N>(result);
  }

  // TODO implement conversions:
  // - toMap: ImmutableArray[std::pair<A, B>] -> ImmutableMap[A, B]
  // - toMap: (f: A -> (K, V)) -> ImmutableMap[K, V]
  // - toSet: ImmutableSet[A]
  // - toVector: ImmutableVector[A]

private:
  std::array<A, N> _data;
};

/** @brief Helper function for creating an 'ImmutableArray'. */
template<
  typename... Values,
  std::size_t N = sizeof...(Values),
  typename A = std::decay_t<std::tuple_element_t<0, std::tuple<Values...>>>
> requires AllSame<Values...>
ImmutableArray<A, N> ImmArray(Values... values) {
  return ImmutableArray<A, N>({values...});
}

/** @brief Helper function for creating an empty 'ImmutableArray'. */
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

template<typename A>
struct DefaultValue<ImmutableArray<A, 0>> {
  static ImmutableArray<A, 0> a() { return EmptyImmArray<A>(); }
};

template<HasSemigroup A, std::size_t N>
struct Semigroup<ImmutableArray<A, N>> {
  static ImmutableArray<A, N> combine(const ImmutableArray<A, N>& a, const ImmutableArray<A, N>& b) {
    std::array<A, N> result;
    for (std::size_t i = 0; i < N; i++) {
      result[i] = Combine(a._unsafe_at(i), b._unsafe_at(i));
    }
    return ImmutableArray<A, N>(result);
  }
};

template<HasStr A, std::size_t N>
struct Str<ImmutableArray<A, N>> {
  static std::string toStr(const ImmutableArray<A, N>& value) {
    std::stringstream ss;
    ss << std::format("ImmutableArray(", N);
    for (std::size_t i = 0; i < N; i++) {
      if (i != 0) ss << ", ";
      ss << ToStr(value._unsafe_at(i));
    }
    ss << ")";
    return ss.str();
  }
};

template<HasDebugStr A, std::size_t N>
struct DebugStr<ImmutableArray<A, N>> {
  static std::string toDebugStr(const ImmutableArray<A, N>& value) {
    std::stringstream ss;
    ss << std::format("ImmutableArray[{}](", N);
    for (std::size_t i = 0; i < N; i++) {
      if (i != 0) ss << ", ";
      ss << std::format("[{}]={}", i, ToDebugStr(value._unsafe_at(i)));
    }
    ss << ")";
    return ss.str();
  }
};

#endif //FPCPP_CORE_DATA_IMMUTABLE_ARRAY_H
