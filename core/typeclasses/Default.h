#ifndef DEFAULT_H
#define DEFAULT_H

#include <string>
#include <concepts>
#include <core/data/Concepts.h>

template<typename A>
struct DefaultValue;

template<typename A>
concept HasDefault = requires() {
  { DefaultValue<A>::a() } -> std::same_as<A>;
};

template<HasDefault A>
A Default() {
  return DefaultValue<A>::a();
}

template<IsArithmetic A>
struct DefaultValue<A> {
  static A a() { return 0; }
};

template<>
struct DefaultValue<std::string> {
  static std::string a() { return {}; }
};

#endif //DEFAULT_H
