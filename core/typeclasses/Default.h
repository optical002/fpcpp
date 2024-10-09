#ifndef FPCPP_CORE_TYPECLASSES_DEFAULT_H
#define FPCPP_CORE_TYPECLASSES_DEFAULT_H

#include <string>
#include <concepts>
#include <core/data/Concepts.h>
#include <core/data/Tag.h>

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
