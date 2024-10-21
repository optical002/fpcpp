#ifndef FPCPP_CORE_DATA_NEW_TYPE_H
#define FPCPP_CORE_DATA_NEW_TYPE_H

#include <core/data/Record.h>
#include <core/macros/Bitmask.h>

#define NEW_TYPE_FLAG_GEN_ORD    (0)(1)
#define NEW_TYPE_FLAG_GEN_NUM    (1)(0)

#define NEW_TYPE_MASK_EMPTY      (0)(0)
#define NEW_TYPE_MASK_ARITHMETIC (1)(1)

/**
 * @brief Wraps a 'Type' into a 'NewType' with a name.
 *
 * It is useful when we want to name a specific type.
 * For example, to avoid mixing up lets say minutes and seconds where both can be 'int's, instead
 * we would create two new types like this:
 * @code
 * GEN_NEW_TYPE(Seconds, int)
 * GEN_NEW_TYPE(Minutes, int)
 * @endcode
 * which would generate:
 * @code
 * struct Seconds {
 * private:
 *   int _a;
 * public:
 *   int a() const { return _a; }
 * };
 * struct Minutes {
 * private:
 *   int _a;
 * public:
 *   int a() const { return _a; }
 * };
 * @endcode
 */
#define GEN_NEW_TYPE_FULL(new_type_name, inline_type, mask) \
  struct new_type_name { \
  private: \
    PRIVATE_FIELDS(inline_type, a) \
  public: \
    PRIVATE_CONSTRUCTOR(new_type_name, inline_type, a) \
    FIELDS_GETTERS(inline_type, a) \
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
  STR_TYPECLASS(new_type_name, inline_type, a) \
  DEBUG_STR_TYPECLASS(new_type_name, inline_type, a) \
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
