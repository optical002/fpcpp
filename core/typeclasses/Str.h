#ifndef FPCPP_CORE_TYPECLASSES_STR_H
#define FPCPP_CORE_TYPECLASSES_STR_H

#include <format>
#include <string>
#include <core/data/Concepts.h>
#include <core/data/Tag.h>

template<typename A>
struct Str;

template<typename A>
concept HasStr = requires(const A& a) {
  { Str<A>::toStr(a) } -> std::same_as<std::string>;
};

template<HasStr A>
std::string ToStr(const A& a) {
  return Str<A>::toStr(a);
}

template<IsArithmetic A>
struct Str<A> {
  static std::string toStr(const A& value) {
    return std::format("{}", value);
  }
};

template<>
struct Str<std::string> {
  static std::string toStr(const std::string& value) {
    return std::format("String({})", value);;
  }
};

template<>
struct Str<bool> {
  static std::string toStr(const bool& value) {
    return std::format("{}", value);
  }
};

template<HasStr A>
struct Str<std::shared_ptr<A>> {
  static std::string toStr(const std::shared_ptr<A>& value) {
    return std::format("SharedPtr(address={:p}, value={}))", static_cast<const void*>(value.get()), ToStr(*value));
  }
};

template<HasStr A, HasTag TagType>
struct Str<Tagged<A, TagType>> {
  static std::string toStr(const Tagged<A, TagType>& value) {
    return ToStr(value.a());
  }
};

#endif // FPCPP_CORE_TYPECLASSES_STR_H