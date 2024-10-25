#ifndef FPCPP_CORE_DATA_IMMUTABLE_MAP_H
#define FPCPP_CORE_DATA_IMMUTABLE_MAP_H

#include <map>
#include <core/data/Option.h>

template<typename Key, typename Value>
struct ImmutableMap {
public:
  template<typename NewValue>
  using NewType = ImmutableMap<Key, NewValue>;
  // TODO add tag and untag
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



#endif //FPCPP_CORE_DATA_IMMUTABLE_MAP_H