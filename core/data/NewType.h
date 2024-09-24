#ifndef FPCPP_CORE_DATA_NEW_TYPE_H
#define FPCPP_CORE_DATA_NEW_TYPE_H

#include <core/data/Record.h>
#include <core/macros/Bitmask.h>

#define NEW_TYPE_FLAG_GEN_ORD    (0)(1)
#define NEW_TYPE_FLAG_GEN_NUM    (1)(0)

#define NEW_TYPE_MASK_EMPTY      (0)(0)
#define NEW_TYPE_MASK_ARITHMETIC (1)(1)

#define GEN_NEW_TYPE_FULL(new_type_name, inline_type, mask) \
  struct new_type_name { \
    CONST_FIELDS(inline_type, a) \
    WITH_FUNCTIONS(new_type_name, inline_type, a) \
    EQ_OPERATORS(new_type_name, inline_type, a) \
    CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, NEW_TYPE_FLAG_GEN_ORD)) ( \
      ORD_OPERATORS(new_type_name, inline_type, a), \
    ) \
    CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, NEW_TYPE_FLAG_GEN_NUM)) ( \
      NUM_OPERATORS(new_type_name, inline_type, a), \
    ) \
    CHAOS_PP_IF(CHAOS_PP_EQUAL(BITMASK_CHECK_FLAG(mask, NEW_TYPE_FLAG_GEN_NUM), 0)) ( \
      SEMIGROUP_OPERATORS(new_type_name, inline_type, a), \
    ) \
  }; \
  EQ_TYPECLASS(new_type_name, inline_type, a) \
  TO_STRING_TYPECLASS(new_type_name, inline_type, a) \
  SEMIGROUP_TYPECLASS(new_type_name, inline_type, a) \
  CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, NEW_TYPE_FLAG_GEN_ORD)) ( \
    ORD_TYPECLASS(new_type_name, inline_type, a), \
  ) \
  CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, NEW_TYPE_FLAG_GEN_NUM)) ( \
    NUM_TYPECLASS(new_type_name, inline_type, a), \
  ) 

#define GEN_NEW_TYPE(new_type_name, inline_type) \
  GEN_NEW_TYPE_FULL(new_type_name, inline_type, NEW_TYPE_MASK_EMPTY) \

#define GEN_NEW_TYPE_ARITHMETIC(new_type_name, inline_type) \
  GEN_NEW_TYPE_FULL(new_type_name, inline_type, NEW_TYPE_MASK_ARITHMETIC)

#endif // FPCPP_CORE_DATA_NEW_TYPE_H
