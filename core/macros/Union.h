#ifndef FPCPP_CORE_MACROS_UNION_H
#define FPCPP_CORE_MACROS_UNION_H

#include <core/macros/Group.h>
#include <core/data/Variant.h>
#include <chaos/preprocessor/control/if.h>
#include <chaos/preprocessor/comparison/equal.h>
#include <chaos/preprocessor/arithmetic/dec.h>
#include <chaos/preprocessor/arithmetic/div.h>
#include <chaos/preprocessor/recursion/expr.h>
#include <chaos/preprocessor/seq/for_each_i.h>
#include <chaos/preprocessor/tuple/size.h>
#include <chaos/preprocessor/stringize.h>

#define UNION_ENUM_NAME(union_name) union_name##Kind

#define UNION_ENUM_SINGLE_FIELD(name, i) name = i
#define UNION_ENUM_SINGLE_FIELD_COMMA(name, i) name = i,
#define UNION_ENUM_SINGLE_FIELD_IMPL(s, i, type, name, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    UNION_ENUM_SINGLE_FIELD(name, i), \
    UNION_ENUM_SINGLE_FIELD_COMMA(name, i) \
  )

#define UNION_ENUM_FIELDS(...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_ENUM_SINGLE_FIELD_IMPL, GROUP_VARIADIC(__VA_ARGS__), CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_ENUM(union_name, ...) \
  enum class UNION_ENUM_NAME(union_name) : std::uint8_t { \
    UNION_ENUM_FIELDS(__VA_ARGS__) \
  };

#define UNION_ENUM_EQ(union_name) \
  template<> \
  struct Eq<UNION_ENUM_NAME(union_name)> { \
    static bool equal(const UNION_ENUM_NAME(union_name)& a, const UNION_ENUM_NAME(union_name)& b) { return a == b; } \
  };

#define UNION_ENUM_TO_STRING_SINGLE_FIELD(s, i, type, name, union_enum_name) \
  case union_enum_name::name: return CHAOS_PP_STRINGIZE(name);

#define UNION_ENUM_TO_STRING_FIELDS(union_name, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_ENUM_TO_STRING_SINGLE_FIELD, GROUP_VARIADIC(__VA_ARGS__), UNION_ENUM_NAME(union_name) \
  ))

#define UNION_ENUM_TO_STRING(union_name, ...) \
  template<> \
  struct ToString<UNION_ENUM_NAME(union_name)> { \
    static std::string toStr(const UNION_ENUM_NAME(union_name)& a) { \
      switch (a) { \
        UNION_ENUM_TO_STRING_FIELDS(union_name, __VA_ARGS__) \
      } \
      throw std::runtime_error("Impossible"); \
    } \
  };

#define UNION_VARIANT_FIELD_SINGLE_TYPE(type) type
#define UNION_VARIANT_FIELD_SINGLE_TYPE_COMMA(type) type,
#define UNION_VARIANT_FIELD_SINGLE_TYPE_IMPL(s, i, type, name, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    UNION_VARIANT_FIELD_SINGLE_TYPE(type), \
    UNION_VARIANT_FIELD_SINGLE_TYPE_COMMA(type) \
  )

#define UNION_VARIANT_FIELD_TYPES(...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_VARIANT_FIELD_SINGLE_TYPE_IMPL, \
    GROUP_VARIADIC(__VA_ARGS__), CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_VARIANT_TYPE(...) \
  Variant<UNION_VARIANT_FIELD_TYPES(__VA_ARGS__)>

#define UNION_VARIANT_TYPE_SEQ_ITERATION_SINGLE(s, i, type, name, size) \
  UNION_VARIANT_FIELD_SINGLE_TYPE_IMPL(s, i, type, name, size)

#define UNION_VARIANT_TYPE_SEQ_ITERATION(seq, size) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_VARIANT_TYPE_SEQ_ITERATION_SINGLE, seq, size \
  ))

#define UNION_VARIANT_TYPE_SEQ(seq, size) \
  Variant<UNION_VARIANT_TYPE_SEQ_ITERATION(seq, size)>

#define UNION_VARIANT_FIELD(...) \
  const UNION_VARIANT_TYPE(__VA_ARGS__) _variant;

#define UNION_PRIVATE_CONSTRUCTOR(union_name, ...) \
  union_name( \
    UNION_VARIANT_TYPE(__VA_ARGS__) v \
  ) : _variant(std::move(v)) {}

#define UNION_SINGLE_STATIC_CONSTRUCTOR(s, i, type, name, union_name, args_seq, size) \
  static union_name create_##name(const type& name) { \
    return {UNION_VARIANT_TYPE_SEQ(args_seq, size)::create<i>(name)}; \
  }

#define UNION_STATIC_CONSTRUCTORS(union_name, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_SINGLE_STATIC_CONSTRUCTOR, GROUP_VARIADIC(__VA_ARGS__), union_name, GROUP_VARIADIC(__VA_ARGS__), CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_GETTER_NAME(name) as_##name

#define UNION_SINGLE_GETTER(s, i, type, name, union_name) \
  [[nodiscard]] Option<type> UNION_GETTER_NAME(name)() const { return _variant.template get<i>(); }

#define UNION_GETTERS(union_name, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_SINGLE_GETTER, GROUP_VARIADIC(__VA_ARGS__), union_name \
  ))

#define UNION_FOLDING_FUNC_NAME(name) on_##name
#define UNION_FOLDING_FUNC_FIELD_NAME(name) _on_##name

#define UNION_FOLDING_SINGLE_TYPENAME(func_name) typename func_name
#define UNION_FOLDING_SINGLE_TYPENAME_COMMA(func_name) typename func_name,
#define UNION_FOLDING_SINGLE_TYPENAME_IMPL(s, i, type, name, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    UNION_FOLDING_SINGLE_TYPENAME(UNION_FOLDING_FUNC_NAME(name)), \
    UNION_FOLDING_SINGLE_TYPENAME_COMMA(UNION_FOLDING_FUNC_NAME(name)) \
  )

#define UNION_FOLDING_TYPENAMES(...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_FOLDING_SINGLE_TYPENAME_IMPL, GROUP_VARIADIC(__VA_ARGS__), \
    CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_FOLDING_B_RESULT(type, name, ...) \
  typename B = std::invoke_result_t<UNION_FOLDING_FUNC_NAME(name), type>

#define UNION_FOLDING_SINGLE_REQUIRES(type, name) \
  std::invocable<UNION_FOLDING_FUNC_NAME(name), type>
#define UNION_FOLDING_SINGLE_REQUIRES_AND(type, name) \
  std::invocable<UNION_FOLDING_FUNC_NAME(name), type> &&

#define UNION_FOLDING_SINGLE_REQUIRES_IMPL(s, i, type, name, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    UNION_FOLDING_SINGLE_REQUIRES(type, name), \
    UNION_FOLDING_SINGLE_REQUIRES_AND(type, name) \
  )

#define UNION_FOLDING_REQUIRES(...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_FOLDING_SINGLE_REQUIRES_IMPL, GROUP_VARIADIC(__VA_ARGS__), \
    CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_FOLDING_SINGLE_ARG(name) UNION_FOLDING_FUNC_NAME(name)&& UNION_FOLDING_FUNC_FIELD_NAME(name)
#define UNION_FOLDING_SINGLE_ARG_COMMA(name) UNION_FOLDING_FUNC_NAME(name)&& UNION_FOLDING_FUNC_FIELD_NAME(name),

#define UNION_FOLDING_SINGLE_ARG_IMPL(s, i, type, name, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    UNION_FOLDING_SINGLE_ARG(name), \
    UNION_FOLDING_SINGLE_ARG_COMMA(name) \
  )

#define UNION_FOLDING_ARGS(...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_FOLDING_SINGLE_ARG_IMPL, GROUP_VARIADIC(__VA_ARGS__), \
    CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_FOLD_SINGLE_SWITCH_CASE(s, i, type, name, union_name) \
  case UNION_ENUM_NAME(union_name)::name: return UNION_FOLDING_FUNC_FIELD_NAME(name)(_variant.template _unsafe_get<i>());

#define UNION_FOLD_SWITCH_CASES(union_name, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_FOLD_SINGLE_SWITCH_CASE, GROUP_VARIADIC(__VA_ARGS__), union_name \
  ))

#define UNION_FOLD(union_name, ...) \
  template< \
    UNION_FOLDING_TYPENAMES(__VA_ARGS__), \
    UNION_FOLDING_B_RESULT(__VA_ARGS__) \
  > requires UNION_FOLDING_REQUIRES(__VA_ARGS__) \
  B fold(UNION_FOLDING_ARGS(__VA_ARGS__)) const { \
    switch (kind()) { \
      UNION_FOLD_SWITCH_CASES(union_name, __VA_ARGS__) \
    } \
    throw std::runtime_error("Impossible"); \
  }

#define UNION_VOID_FOLD_SINGLE_SWITCH_CASE(s, i, type, name, union_name) \
  case UNION_ENUM_NAME(union_name)::name: UNION_FOLDING_FUNC_FIELD_NAME(name)(_variant.template _unsafe_get<i>()); break;

#define UNION_VOID_FOLD_SWITCH_CASES(union_name, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_VOID_FOLD_SINGLE_SWITCH_CASE, GROUP_VARIADIC(__VA_ARGS__), union_name \
  ))

#define UNION_VOID_FOLD(union_name, ...) \
  template< \
    UNION_FOLDING_TYPENAMES(__VA_ARGS__) \
  > requires UNION_FOLDING_REQUIRES(__VA_ARGS__) \
  void voidFold(UNION_FOLDING_ARGS(__VA_ARGS__)) const { \
    switch (kind()) { \
      UNION_VOID_FOLD_SWITCH_CASES(union_name, __VA_ARGS__) \
    } \
  }

#define UNION_EQ_SINGLE_EQUAL_DATA(name) Equal(a.UNION_GETTER_NAME(name)(), b.UNION_GETTER_NAME(name)())
#define UNION_EQ_SINGLE_EQUAL_DATA_AND(name) Equal(a.UNION_GETTER_NAME(name)(), b.UNION_GETTER_NAME(name)()) &&

#define UNION_EQ_SINGLE_EQUAL_DATA_IMPL(s, i, type, name, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    UNION_EQ_SINGLE_EQUAL_DATA(name), \
    UNION_EQ_SINGLE_EQUAL_DATA_AND(name) \
  )

#define UNION_EQ_EQUAL_DATA(...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_EQ_SINGLE_EQUAL_DATA_IMPL, GROUP_VARIADIC(__VA_ARGS__), \
    CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_EQ(union_name, ...) \
  template<> \
  struct Eq<union_name> { \
    static bool equal(const union_name& a, const union_name& b) { \
      return Equal(a.kind(), b.kind()) && equalData(a, b); \
    } \
  private: \
    static bool equalData(const union_name& a, const union_name& b) { \
      return UNION_EQ_EQUAL_DATA(__VA_ARGS__); \
    } \
  };

#define UNION_TO_STRING_FOLD_SINGLE(type, name) [](const type& name) { return ToStr(name); }
#define UNION_TO_STRING_FOLD_SINGLE_COMMA(type, name) [](const type& name) { return ToStr(name); },

#define UNION_TO_STRING_FOLD_SINGLE_IMPL(s, i, type, name, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    UNION_TO_STRING_FOLD_SINGLE(type, name), \
    UNION_TO_STRING_FOLD_SINGLE_COMMA(type, name) \
  )

#define UNION_TO_STRING_FOLD(...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_TO_STRING_FOLD_SINGLE_IMPL, GROUP_VARIADIC(__VA_ARGS__), \
    CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_TO_STRING(union_name, ...) \
  template<> \
  struct ToString<union_name> { \
    static std::string toStr(const union_name& a) { \
      return std::format(CHAOS_PP_STRINGIZE(union_name(kind={}, data={})), ToStr(a.kind()), toDataString(a));\
    } \
  private: \
    static std::string toDataString(const union_name& a) { \
      return a.fold(UNION_TO_STRING_FOLD(__VA_ARGS__)); \
    } \
  };

#define UNION_SINGLE_STATIC_CONSTRUCTOR_TEMPLATE(s, i, type, name, union_name, template_typename_name, args_seq, size) \
  static union_name<template_typename_name> create_##name(const type& name) { \
    return {UNION_VARIANT_TYPE_SEQ(args_seq, size)::template create<i>(name)}; \
  }

#define UNION_STATIC_CONSTRUCTORS_TEMPLATE(union_name, template_typename_name, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    UNION_SINGLE_STATIC_CONSTRUCTOR_TEMPLATE, GROUP_VARIADIC(__VA_ARGS__), union_name, template_typename_name, GROUP_VARIADIC(__VA_ARGS__), CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define UNION_EQ_TEMPLATE(union_name, template_typename_name, ...) \
  template<typename template_typename_name> \
  struct Eq<union_name<template_typename_name>> { \
    static bool equal(const union_name<template_typename_name>& a, const union_name<template_typename_name>& b) { \
      return Equal(a.kind(), b.kind()) && equalData(a, b); \
    } \
  private: \
    static bool equalData(const union_name<template_typename_name>& a, const union_name<template_typename_name>& b) { \
      return UNION_EQ_EQUAL_DATA(__VA_ARGS__); \
    } \
  };

#define UNION_TO_STRING_TEMPLATE(union_name, template_typename_name, ...) \
  template<typename template_typename_name> \
  struct ToString<union_name<template_typename_name>> { \
    static std::string toStr(const union_name<template_typename_name>& a) { \
      return std::format(CHAOS_PP_STRINGIZE(union_name(kind={}, data={})), ToStr(a.kind()), toDataString(a));\
    } \
  private: \
    static std::string toDataString(const union_name<template_typename_name>& a) { \
      return a.fold(UNION_TO_STRING_FOLD(__VA_ARGS__)); \
    } \
  };

#define UNION_ENUM_KIND_FIELD(union_name) \
  [[nodiscard]] UNION_ENUM_NAME(union_name) kind() const { return static_cast<UNION_ENUM_NAME(union_name)>(_variant.index()); }

#endif // FPCPP_CORE_MACROS_UNION_H