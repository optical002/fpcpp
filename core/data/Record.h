#pragma once

#include <string>
#include <format>
#include <core/macros/General.h>
#include <core/macros/Typeclasses.h>
#include <core/macros/Withers.h>
#include <core/typeclasses/Eq.h>
#include <core/typeclasses/ToString.h>
  
/**
 * @brief Generates a read-only data structure.
 * 
 * It's the 'Product Type' of ADT(Algebraic Data Types) also referred to
 * as a Record in other programming languages.
 *
 *
 * if we want to generate a structure like this:
 * @code
 * struct Testing {
 *   const int age;
 *   const float cost;
 *   const std::string name;
 * }
 * @endcode
 * we would write like this:
 * @code
 * GENERATE_RECORD(Testing,
 *   int, age,
 *   float, cost,
 *   std::string, name
 * )
 * @endcode
 * Implemented typeclasses:
 * - Eq
 * - ToString
 * 
 * Implemented function:
 * - Equal operators: ==, !=
 * - With functions.
 * @param RECORD_NAME The name of the record.
 * @param ... The fields of the record. They come in pairs of type and name (e.g. int, age).
 */ 
#define GENERATE_RECORD(record_name, ...) \
  struct record_name { \
    __VA_OPT__(CONST_FIELDS(__VA_ARGS__)) \
    EQ_OPERATORS(record_name, __VA_ARGS__) \
    __VA_OPT__(WITH_FUNCTIONS(record_name, __VA_ARGS__)) \
  }; \
  EQ_TYPECLASS(record_name, __VA_ARGS__) \
  TO_STRING_TYPECLASS(record_name, __VA_ARGS__)

