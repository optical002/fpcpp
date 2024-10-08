#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <tuple>

template<typename... Args>
auto Tpl(Args&&... args) {
  return std::make_tuple(std::forward<Args>(args)...);
}

// template<typename Key, typename Value>
// std::pair<Key, Value> KV(Key&& key, Value&& value) {
//   return std::make_pair(std::forward<Key>(key), std::forward<Value>(value));
// }

#endif //FUNCTIONS_H
