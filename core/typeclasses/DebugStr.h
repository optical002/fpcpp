#ifndef FPCPP_CORE_TYPECLASSES_DEBUGSTR_H
#define FPCPP_CORE_TYPECLASSES_DEBUGSTR_H

#include <format>
#include <string>
#include <core/data/Concepts.h>
#include <core/data/Tag.h>

template<typename A>
struct DebugStr;

template<typename A>
concept HasDebugStr = requires(const A& a) {
  { DebugStr<A>::toDebugStr(a) } -> std::same_as<std::string>;
};

template<HasDebugStr A>
std::string ToDebugStr(const A& a) {
  return DebugStr<A>::toDebugStr(a);
}

template<IsArithmetic A>
struct DebugStr<A> {
  static std::string toDebugStr(const A& value) {
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
struct DebugStr<std::string> {
  static std::string toDebugStr(const std::string& value) {
    return std::format("String({})", value);
  }
};

template<>
struct DebugStr<bool> {
  static std::string toDebugStr(const bool& value) {
    return std::format("Bool({})", value);
  }
};

template<HasDebugStr A>
struct DebugStr<std::shared_ptr<A>> {
  static std::string toDebugStr(const std::shared_ptr<A>& value) {
    return std::format("SharedPtr(address={:p}, value={}))", static_cast<const void*>(value.get()), ToStr(*value));
  }
};

template<HasDebugStr A, HasTag TagType>
struct DebugStr<Tagged<A, TagType>> {
  static std::string toDebugStr(const Tagged<A, TagType>& value) {
    return ToStr(value.a());
  }
};

#endif //FPCPP_CORE_TYPECLASSES_DEBUGSTR_H
