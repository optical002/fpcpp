#ifndef FPCPP_CORE_DATA_RECORD_H
#define FPCPP_CORE_DATA_RECORD_H

#include <string>
#include <format>
#include <core/macros/General.h>
#include <core/macros/Typeclasses.h>
#include <core/macros/Withers.h>
#include <core/typeclasses/Eq.h>
#include <core/typeclasses/Str.h>
#include <core/typeclasses/DebugStr.h>

#define RECORD_FLAG_GEN_EQ_TYPECLASS        (0)(0)(1)
#define RECORD_FLAG_GEN_STR_TYPECLASS       (0)(1)(0)
#define RECORD_FLAG_GEN_DEBUG_STR_TYPECLASS (1)(0)(0)

#define RECORD_MASK_NO_TYPECLASSES          (0)(0)(0)
#define RECORD_MASK_GEN_ALL                 (1)(1)(1)

#define GEN_RECORD_FULL(mask, record_name, ...) \
  struct record_name { \
  private: \
    __VA_OPT__(PRIVATE_FIELDS(__VA_ARGS__)) \
  public: \
    __VA_OPT__(PRIVATE_CONSTRUCTOR(record_name, __VA_ARGS__)) \
    __VA_OPT__(FIELDS_GETTERS(__VA_ARGS__)) \
    CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, RECORD_FLAG_GEN_EQ_TYPECLASS)) ( \
      EQ_OPERATORS(record_name, __VA_ARGS__) \
    ) \
    __VA_OPT__(WITH_FUNCTIONS(record_name, __VA_ARGS__)) \
  }; \
  CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, RECORD_FLAG_GEN_EQ_TYPECLASS)) ( \
    EQ_TYPECLASS(record_name, __VA_ARGS__) \
  ) \
  CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, RECORD_FLAG_GEN_STR_TYPECLASS)) ( \
    STR_TYPECLASS(record_name, __VA_ARGS__) \
  ) \
  CHAOS_PP_IF(BITMASK_CHECK_FLAG(mask, RECORD_FLAG_GEN_DEBUG_STR_TYPECLASS)) ( \
    DEBUG_STR_TYPECLASS(record_name, __VA_ARGS__) \
  )

/**
 * @brief Generates a read-only data structure.
 * 
 * It's the 'Product Type' of ADT(Algebraic Data Types) also referred to
 * as a Record in other programming languages.
 *
 *
 * Given the following record definition:
 * @code
 * GEN_RECORD(Testing,
 *   int, intFieldName,
 *   float, floatFieldName,
 *   std::string, stringFieldName
 * )
 * @endcode
 * It will generate the following struct.
 * @code
 * struct Testing {
 *   const int intFieldName;
 *   const float floatFieldName;
 *   const std::string stringFieldName;
 * }
 * @endcode
 *
 * Additionally it will implement:
 * - Functions:
 *   - Equal operators: ==, !=
 *   - With Constructors
 * - Typeclasses:
 *   - Eq
 *   - Str
 *   - DebugStr
 * @param RECORD_NAME The name of the record.
 * @param ... The fields of the record. They come in pairs of type and name (e.g. 'int, age').
 */ 
#define GEN_RECORD(record_name, ...) \
  GEN_RECORD_FULL(RECORD_MASK_GEN_ALL, record_name, __VA_ARGS__)

#define GEN_RECORD_NO_TYPECLASSES(record_name, ...) \
  GEN_RECORD_FULL(RECORD_MASK_NO_TYPECLASSES, record_name, __VA_ARGS__)

#endif // FPCPP_CORE_DATA_RECORD_H