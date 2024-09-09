// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros
#pragma once

#include <core/typeclasses/Eq.h>
#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Ord.h>
#include <chaos/preprocessor.h>

#define EQ_OPERATOR_SINGLE_FIELD(type, name) Equal(name, other.name)
#define EQ_OPERATOR_FIELDS(...) FOR_EACH2_CON(EQ_OPERATOR_SINGLE_FIELD, &&, __VA_ARGS__)
#define EQ_OPERATORS(type, ...) \
  bool operator==(const type& other) const { \
    return \
      CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 1)) ( \
        true, \
        EQ_OPERATOR_FIELDS(__VA_ARGS__) \
      ); \
  } \
  bool operator!=(const type& other) const { \
    return !(*this == other); \
  }

#define EQ_TYPECLASS_SINGLE_FIELD(type, name) Equal(a.name, b.name)
#define EQ_TYPECLASS_FIELDS(...) FOR_EACH2_CON(EQ_TYPECLASS_SINGLE_FIELD, &&, __VA_ARGS__)
#define EQ_TYPECLASS(type, ...) \
  template<> \
  struct Eq<type> { \
    static bool equal(const type& a, const type& b) { \
      return \
        CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 1)) ( \
          true, \
          EQ_TYPECLASS_FIELDS(__VA_ARGS__) \
        ); \
    } \
  };

#define TO_STRING_STR_SINGLE_FIELD(type, name) name={}
#define TO_STRING_STR_FIELDS_IMPL(...) FOR_EACH2_COMMA(TO_STRING_STR_SINGLE_FIELD, __VA_ARGS__)
#define TO_STRING_STR_FIELDS(type, ...) EXPAND(DEFER(STRINGIFY) (type(TO_STRING_STR_FIELDS_IMPL(__VA_ARGS__))))

#define TO_STRING_SINGLE_VALUE(type, name) ToStr(a.name)
#define TO_STRING_VALUES(...) FOR_EACH2_COMMA(TO_STRING_SINGLE_VALUE, __VA_ARGS__)

#define TO_STRING_TYPECLASS(type, ...) \
  template<> \
  struct ToString<type> { \
    static std::string toStr(const type& a) { \
      return \
        CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 1)) ( \
          STRINGIFY(type##()), \
          std::format(TO_STRING_STR_FIELDS(type, __VA_ARGS__), TO_STRING_VALUES(__VA_ARGS__)) \
        ); \
    } \
  };

#define ORD_OPERATORS(record_type, field_type, field_name) \
  bool operator<(const record_type& other) const { \
    return Compare(field_name, other.field_name) < 0; \
  } \
  bool operator<=(const record_type& other) const { \
    return Compare(field_name, other.field_name) <= 0; \
  } \
  bool operator>(const record_type& other) const { \
    return Compare(field_name, other.field_name) > 0; \
  } \
  bool operator>=(const record_type& other) const { \
    return Compare(field_name, other.field_name) >= 0; \
  }

#define ORD_TYPECLASS(record_type, field_type, field_name) \
  template<> \
  struct Ord<record_type> { \
    static int compare(const record_type& a, const record_type& b) { \
      return Compare(a.field_name, b.field_name); \
    } \
  };