#pragma once

#include <core/data/Record.h>
#include <core/macros/Bitmask.h>

// Constructor flags.
#define NEW_TYPE_FLAG_EMPTY   (0)(0)(0)(0)
#define NEW_TYPE_FLAG_GEN_ORD (0)(0)(0)(1)

#define GEN_NEW_TYPE_CONSTRUCTOR_FLAGS(new_type_name, inline_type, mask) \
  struct new_type_name { \
    CONST_FIELDS(inline_type, a) \
    EQ_OPERATORS(new_type_name, inline_type, a) \
    CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, NEW_TYPE_FLAG_GEN_ORD)) ( \
      ORD_OPERATORS(new_type_name, inline_type, a), \
    ) \
    WITH_FUNCTIONS(new_type_name, inline_type, a) \
  }; \
  EQ_TYPECLASS(new_type_name, inline_type, a) \
  TO_STRING_TYPECLASS(new_type_name, inline_type, a) \
  CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, NEW_TYPE_FLAG_GEN_ORD)) ( \
    ORD_TYPECLASS(new_type_name, inline_type, a), \
  )

#define GEN_NEW_TYPE(new_type_name, inline_type) \
  GEN_NEW_TYPE_CONSTRUCTOR_FLAGS(new_type_name, inline_type, NEW_TYPE_FLAG_EMPTY) \

#define GEN_NEW_TYPE_ORD(new_type_name, inline_type) \
  GEN_NEW_TYPE_CONSTRUCTOR_FLAGS(new_type_name, inline_type, NEW_TYPE_FLAG_GEN_ORD)

GEN_NEW_TYPE_ORD(Age, int)