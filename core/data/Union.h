#ifndef FPCPP_CORE_DATA_UNION_H
#define FPCPP_CORE_DATA_UNION_H

#include <string>
#include <utility>
#include <core/typeclasses/Default.h>
#include <core/typeclasses/Str.h>
#include <core/typeclasses/DebugStr.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Option.h>
#include <core/macros/Union.h>

#define GEN_UNION(union_name, ...) \
  UNION_ENUM(union_name, __VA_ARGS__) \
  UNION_ENUM_EQ(union_name) \
  UNION_ENUM_STR(union_name, __VA_ARGS__) \
  UNION_ENUM_DEBUG_STR(union_name, __VA_ARGS__) \
  struct union_name { \
  private: \
    UNION_VARIANT_FIELD(__VA_ARGS__) \
    UNION_PRIVATE_CONSTRUCTOR(union_name, __VA_ARGS__) \
  public: \
    UNION_ENUM_KIND_FIELD(union_name) \
    UNION_STATIC_CONSTRUCTORS(union_name, __VA_ARGS__) \
    UNION_GETTERS(union_name, __VA_ARGS__) \
    UNION_FOLD(union_name, __VA_ARGS__) \
    UNION_VOID_FOLD(union_name, __VA_ARGS__) \
  }; \
  UNION_EQ(union_name, __VA_ARGS__) \
  UNION_STR(union_name, __VA_ARGS__) \
  UNION_DEBUG_STR(union_name, __VA_ARGS__)

#define GEN_UNION_TEMPLATE(union_name, template_typename_name, ...) \
  UNION_ENUM(union_name, __VA_ARGS__) \
  UNION_ENUM_EQ(union_name) \
  UNION_ENUM_STR(union_name, __VA_ARGS__) \
  UNION_ENUM_DEBUG_STR(union_name, __VA_ARGS__) \
  template<typename template_typename_name> \
  struct union_name { \
  private: \
    UNION_VARIANT_FIELD(__VA_ARGS__) \
    UNION_PRIVATE_CONSTRUCTOR(union_name, __VA_ARGS__) \
  public: \
    UNION_ENUM_KIND_FIELD(union_name) \
    UNION_STATIC_CONSTRUCTORS_TEMPLATE(union_name, template_typename_name, __VA_ARGS__) \
    UNION_GETTERS(union_name, __VA_ARGS__) \
    UNION_FOLD(union_name, __VA_ARGS__) \
    UNION_VOID_FOLD(union_name, __VA_ARGS__) \
  };  \
  UNION_EQ_TEMPLATE(union_name, template_typename_name, __VA_ARGS__) \
  UNION_STR_TEMPLATE(union_name, template_typename_name, __VA_ARGS__) \
  UNION_DEBUG_STR_TEMPLATE(union_name, template_typename_name, __VA_ARGS__) \

#endif // FPCPP_CORE_DATA_UNION_H
