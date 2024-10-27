#ifndef FPCPP_CORE_DATA_IMMUTABLE_MAP_H
#define FPCPP_CORE_DATA_IMMUTABLE_MAP_H

#include <map>
#include <core/data/Option.h>

template<typename Key, typename Value>
struct ImmutableMap {
public:
  template<typename NewValue>
  using NewType = ImmutableMap<Key, NewValue>;
  using ValueType = Value;
  using KeyType = Key;

  explicit ImmutableMap(const std::map<Key, Value>& map) : _map(map) {}

  Value _unsafe_at(const Key& key) const {
    return _map.at(key);
  }
  Option<Value> at(const Key& key) const {
    auto it = _map.find(key);
    return it == _map.end() ? None : Some(it->second);
  }
  Option<Value> operator[](const Key& key) const {
    return at(key);
  }

  [[nodiscard]] std::size_t size() const { return _map.size(); }

  [[nodiscard]] bool isEmpty() const { return size() == 0; }

  typename std::map<Key, Value>::const_iterator begin() const {
    return _map.cbegin();
  }

  typename std::map<Key, Value>::const_iterator end() const {
    return _map.cend();
  }

  template<
    std::invocable<Value> Func,
    typename NewValue = std::invoke_result_t<Func, Value>
  >
  ImmutableMap<Key, NewValue> map(Func&& f) const {
    std::map<Key, NewValue> result;

    for (auto& [key, value] : _map) {
      result[key] = f(value);
    }

    return ImmutableMap<Key, NewValue>(result);
  }

  template<
    std::invocable<Value> Func,
    typename NewMap = std::invoke_result_t<Func, Value>,
    typename NewValue = typename NewMap::ValueType,
    typename NewKey = typename NewMap::KeyType
  >
  ImmutableMap<NewKey, NewValue> flatMap(Func&& f) const {
    std::map<NewKey, NewValue> result;

    for (auto& [_, value] : _map) {
      for (
        const auto innerMap = f(value);
        auto& [innerKey, innerValue] : innerMap
      ) {
        result[innerKey] = innerValue;
      }
    }

    return ImmutableMap<NewKey, NewValue>(result);
  }

  // TODO implement operations:
  // - flatten: [K, Iterable[V]] -> Iterable[(K, V)]
  // - reduce: (f: (K, V) -> B) : B
  // - fold: (initial: B, f: (B, K, V) -> B) : B
  // - find: (f: (K, V) -> bool) : Option[(K, V)]
  // - exists: (f: (K, V) -> bool) : bool
  // - forAll: (f: (K, V) -> bool) : bool
  // - filter: (f: (K, V) -> bool) : Map[(K, V)]
  // - mkString: (separator: String) : String
  // - mkString: (f: (K, V) -> String) : String

  // TODO implement conversions:
  // - toSet: ImmutableSet[(K, V)]
  // - toVector: ImmutableVector[(K, V)]
  // - keys: ImmutableSet[K]
  // - values: ImmutableVector[V]

  // TODO implement typeclasses:
  // - Eq
  // - Default
  // - Semigroup
  // - Str
  // - DebugStr

  // TODO add tag and untag

private:
  std::map<Key, Value> _map;
};

template<
  typename... Pairs,
  typename Key = typename std::decay_t<std::tuple_element_t<0, std::tuple<Pairs...>>>::first_type,
  typename Value = typename std::decay_t<std::tuple_element_t<0, std::tuple<Pairs...>>>::second_type
>
ImmutableMap<Key, Value> ImmMap(Pairs... pairs) {
  return ImmutableMap<Key, Value>({pairs...});
}

template<typename Key, typename Value>
ImmutableMap<Key, Value> EmptyImmMap() { return ImmutableMap<Key, Value>({}); }



#endif //FPCPP_CORE_DATA_IMMUTABLE_MAP_H
