#pragma once

#include <concepts>
#include <type_traits>
#include <string>

template<typename A, typename = void>
struct Eq;

template<typename A>
concept HasEq = requires(const A& a, const A& b) {
  { Eq<A>::equal(a, b) } -> std::same_as<bool>;
};

template<typename A> requires HasEq<A>
bool Equal(const A& a, const A& b) {
  return Eq<A>::equal(a, b);
}

template<typename A>
struct Eq<A, std::enable_if_t<std::is_arithmetic_v<A>>> {
  static bool equal(const A& a, const A& b) {
    return a == b;
  }
};

template<>
struct Eq<std::string> {
  static bool equal(const std::string& a, const std::string& b) {
    return a == b;
  }
};
