#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <tuple>

template<typename... Args>
auto Tpl(Args&&... args) {
  return std::make_tuple(std::forward<Args>(args)...);
}

#endif //FUNCTIONS_H
