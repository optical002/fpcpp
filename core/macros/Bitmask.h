// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros
#ifndef FPCPP_CORE_MACROS_BITMASK_H
#define FPCPP_CORE_MACROS_BITMASK_H

#include <core/macros/ToSeq.h>
#include <chaos/preprocessor/logical/bitand.h>
#include <chaos/preprocessor/logical/bitor.h>
#include <chaos/preprocessor/seq/elem.h>
#include <chaos/preprocessor/seq/size.h>
#include <chaos/preprocessor/seq/for_each_i.h>
#include <chaos/preprocessor/seq/fold_left.h>
#include <chaos/preprocessor/control/if.h>
#include <chaos/preprocessor/comparison/equal.h>
#include <chaos/preprocessor/comparison/greater_equal.h>
#include <chaos/preprocessor/recursion/expr.h>
#include <chaos/preprocessor/tuple/size.h>
#include <chaos/preprocessor/tuple/elem.h>

#define BITWISE_AND_OP(s, i, elem, right_bits) (CHAOS_PP_BITAND(elem)(CHAOS_PP_SEQ_ELEM(i, right_bits)))
#define BITWISE_AND(left_bits, right_bits) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_SEQ_SIZE(left_bits), CHAOS_PP_SEQ_SIZE(right_bits))) ( \
    CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
      BITWISE_AND_OP, left_bits, right_bits \
    )), \
    "Error[BITWISE_AND]: left_bits and right_bits size does not match" \
  )

#define BITWISE_OR_OP(s, i, elem, right_bits) (CHAOS_PP_BITOR(elem)(CHAOS_PP_SEQ_ELEM(i, right_bits)))
#define BITWISE_OR(left_bits, right_bits) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_SEQ_SIZE(left_bits), CHAOS_PP_SEQ_SIZE(right_bits))) ( \
    CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
      BITWISE_OR_OP, left_bits, right_bits \
    )), \
    "Error[BITWISE_OR]: left_bits and right_bits size does not match" \
  )

#define BITWISE_OR_ARRAY_OP(s, elem_bits, state_bits) BITWISE_OR(elem_bits, state_bits)
#define BITWISE_OR_ARRAY(...) \
  CHAOS_PP_IF(CHAOS_PP_GREATER_EQUAL(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2)) ( \
    CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOLD_LEFT( \
      BITWISE_OR_ARRAY_OP, TO_SEQ_VARIADIC(__VA_ARGS__), CHAOS_PP_VARIADIC_ELEM(0, __VA_ARGS__)\
    )), \
    "Error[BITWISE_OR_ARRAY]: received less than 2 variadic arguments." \
  )

#define BIT_SEQ_OR_OP(s, elem_bit, state_bit) CHAOS_PP_BITOR(elem_bit)(state_bit)
#define BIT_SEQ_OR(bit_seq) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOLD_LEFT( \
    BIT_SEQ_OR_OP, bit_seq, CHAOS_PP_SEQ_ELEM(0, bit_seq) \
  ))

#define BIT_SEQ_AND_OP(s, elem_bit, state_bit) CHAOS_PP_BITAND(elem_bit)(state_bit)
#define BIT_SEQ_AND(bit_seq) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOLD_LEFT( \
    BIT_SEQ_AND_OP, bit_seq, CHAOS_PP_SEQ_ELEM(0, bit_seq) \
  ))

#define BITMASK_CHECK_FLAG(mask, flag) BIT_SEQ_OR(BITWISE_AND(mask, flag))

#endif // FPCPP_CORE_MACROS_BITMASK_H