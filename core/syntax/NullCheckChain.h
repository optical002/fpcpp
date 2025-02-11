#ifndef FPCPP_CORE_SYNTAX_NULL_CHECK_CHAIN_H
#define FPCPP_CORE_SYNTAX_NULL_CHECK_CHAIN_H

#include <type_traits>
#include <memory>

template<
  typename A1,
  typename GetA2, typename A2 = decltype(std::declval<GetA2>()(std::declval<A1>()))
> requires std::invocable<GetA2, A1> && std::is_pointer_v<A1> && std::is_pointer_v<A2>
A2 NullCheckChain(A1 a, GetA2&& getA2) {
  if (a == nullptr) { return nullptr; }
  auto a2 = getA2(a); if (a2 == nullptr) { return nullptr; } return a2;
}

template<
  typename A1,
  typename GetA2, typename A2 = decltype(std::declval<GetA2>()(std::declval<A1>())),
  typename GetA3, typename A3 = decltype(std::declval<GetA3>()(std::declval<A2>()))
> requires
  std::invocable<GetA2, A1>
  && std::invocable<GetA3, A2>
  && std::is_pointer_v<A1> && std::is_pointer_v<A2> && std::is_pointer_v<A3>
A3 NullCheckChain(A1 a, GetA2&& getA2, GetA3&& getA3) {
  if (a == nullptr) { return nullptr; }
  auto a2 = getA2(a); if (a2 == nullptr) { return nullptr; }
  auto a3 = getA3(a2); if (a3 == nullptr) { return nullptr; } return a3;
}

template<
  typename A1,
  typename GetA2, typename A2 = decltype(std::declval<GetA2>()(std::declval<A1>())),
  typename GetA3, typename A3 = decltype(std::declval<GetA3>()(std::declval<A2>())),
  typename GetA4, typename A4 = decltype(std::declval<GetA4>()(std::declval<A3>()))
> requires
  std::invocable<GetA2, A1>
  && std::invocable<GetA3, A2>
  && std::invocable<GetA4, A3>
  && std::is_pointer_v<A1> && std::is_pointer_v<A2> && std::is_pointer_v<A3> && std::is_pointer_v<A4>
A4 NullCheckChain(A1 a, GetA2&& getA2, GetA3&& getA3, GetA4&& getA4) {
  if (a == nullptr) { return nullptr; }
  auto a2 = getA2(a); if (a2 == nullptr) { return nullptr; }
  auto a3 = getA3(a2); if (a3 == nullptr) { return nullptr; }
  auto a4 = getA4(a3); if (a4 == nullptr) { return nullptr; } return a4;
}

#endif //FPCPP_CORE_SYNTAX_NULL_CHECK_CHAIN_H
