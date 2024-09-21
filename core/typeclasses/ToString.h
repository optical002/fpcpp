#ifndef FPCPP_CORE_TYPECLASSES_TO_STRING_H
#define FPCPP_CORE_TYPECLASSES_TO_STRING_H

#include <format>
#include <string>
#include <core/data/Concepts.h>

template<typename A>
struct ToString;

template<typename A>
concept HasToString = requires(const A& a) {
  { ToString<A>::toStr(a) } -> std::same_as<std::string>;
};

template<HasToString A>
std::string ToStr(const A& a) {
  return ToString<A>::toStr(a);
}

template<IsArithmetic A>
struct ToString<A> {
  static std::string toStr(const A& value) {
    std::string typeName;
    if constexpr (std::is_same_v<A, int>) typeName = "Int";
    else if constexpr (std::is_same_v<A, unsigned int>) typeName = "UnsignedInt";
    else if constexpr (std::is_same_v<A, long>) typeName = "Long";
    else if constexpr (std::is_same_v<A, unsigned long>) typeName = "UnsignedLong";
    else if constexpr (std::is_same_v<A, long long>) typeName = "LongLong";
    else if constexpr (std::is_same_v<A, unsigned long long>) typeName = "UnsignedLongLong";
    else if constexpr (std::is_same_v<A, short>) typeName = "Short";
    else if constexpr (std::is_same_v<A, unsigned short>) typeName = "UnsignedShort";
    else if constexpr (std::is_same_v<A, char>) typeName = "Char";
    else if constexpr (std::is_same_v<A, bool>) typeName = "Bool";
    else if constexpr (std::is_same_v<A, float>) typeName = "Float";
    else if constexpr (std::is_same_v<A, double>) typeName = "Double";
    else if constexpr (std::is_same_v<A, long double>) typeName = "LongDouble";
    else {
      static_assert(std::is_same_v<A, void>, "No partial specialization for type");
      throw std::invalid_argument("No partial specialization for type");
    }
    return std::format("{}({})", typeName, value);
  }
};

template<>
struct ToString<std::string> {
  static std::string toStr(const std::string& value) {
    return std::format("String({})", value);
  }
};

#endif // FPCPP_CORE_TYPECLASSES_TO_STRING_H