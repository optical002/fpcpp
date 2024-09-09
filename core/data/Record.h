﻿#pragma once

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
 *   - ToString
 * @param RECORD_NAME The name of the record.
 * @param ... The fields of the record. They come in pairs of type and name (e.g. 'int, age').
 */ 
#define GEN_RECORD(record_name, ...) \
  struct record_name { \
    __VA_OPT__(CONST_FIELDS(__VA_ARGS__)) \
    EQ_OPERATORS(record_name, __VA_ARGS__) \
    __VA_OPT__(WITH_FUNCTIONS(record_name, __VA_ARGS__)) \
  }; \
  EQ_TYPECLASS(record_name, __VA_ARGS__) \
  TO_STRING_TYPECLASS(record_name, __VA_ARGS__)