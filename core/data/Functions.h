#ifndef FPCPP_CORE_DATA_FUNCTIONS_H
#define FPCPP_CORE_DATA_FUNCTIONS_H
#include <tuple>

/** @brief Helper function for creating any size 'std::tuple'. */
template<typename... Args>
auto Tpl(Args&&... args) {
  return std::make_tuple(std::forward<Args>(args)...);
}

// template<typename Key, typename Value>
// std::pair<Key, Value> KV(Key&& key, Value&& value) {
//   return std::make_pair(std::forward<Key>(key), std::forward<Value>(value));
// }

#endif //FPCPP_CORE_DATA_FUNCTIONS_H
