#ifndef FPCPP_CORE_DATA_UNION_H
#define FPCPP_CORE_DATA_UNION_H

#include <string>
#include <utility>
#include <core/typeclasses/Default.h>
#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Option.h>
#include <core/macros/Union.h>

#define GEN_UNION(union_name, ...) \
  UNION_ENUM(union_name, __VA_ARGS__) \
  UNION_ENUM_EQ(union_name) \
  UNION_ENUM_TO_STRING(union_name, __VA_ARGS__) \
  struct union_name { \
    const UNION_ENUM_NAME(union_name) kind; \
  private: \
    UNION_VARIANT_FIELD(__VA_ARGS__) \
    UNION_PRIVATE_CONSTRUCTOR(union_name, __VA_ARGS__) \
  public: \
    UNION_STATIC_CONSTRUCTORS(union_name, __VA_ARGS__) \
    UNION_GETTERS(union_name, __VA_ARGS__) \
    UNION_FOLD(union_name, __VA_ARGS__) \
    UNION_VOID_FOLD(union_name, __VA_ARGS__) \
  }; \
  UNION_EQ(union_name, __VA_ARGS__) \
  UNION_TO_STRING(union_name, __VA_ARGS__)

#define GEN_UNION_TEMPLATE(union_name, template_typename_name, ...) \
  UNION_ENUM(union_name, __VA_ARGS__) \
  UNION_ENUM_EQ(union_name) \
  UNION_ENUM_TO_STRING(union_name, __VA_ARGS__) \
  template<typename template_typename_name> \
  struct union_name { \
    const UNION_ENUM_NAME(union_name) kind; \
  private: \
    UNION_VARIANT_FIELD(__VA_ARGS__) \
    UNION_PRIVATE_CONSTRUCTOR(union_name, __VA_ARGS__) \
  public: \
    UNION_STATIC_CONSTRUCTORS_TEMPLATE(union_name, template_typename_name, __VA_ARGS__) \
    UNION_GETTERS(union_name, __VA_ARGS__) \
    UNION_FOLD(union_name, __VA_ARGS__) \
    UNION_VOID_FOLD(union_name, __VA_ARGS__) \
  };  \
  UNION_EQ_TEMPLATE(union_name, template_typename_name, __VA_ARGS__) \
  UNION_TO_STRING_TEMPLATE(union_name, template_typename_name, __VA_ARGS__)

#endif // FPCPP_CORE_DATA_UNION_H
