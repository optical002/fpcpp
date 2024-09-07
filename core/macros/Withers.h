﻿// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros
#pragma once
#include <core/macros/Group.h>
#include <chaos/preprocessor.h>

#define NAME(s, i, type, name, data) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(i, data)) ( \
    CHAOS_PP_CAT(name, _), \
    name \
  )
#define NAME_COMMA(s, i, type, name, data) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(i, data)) ( \
    CHAOS_PP_CAT(name, _), \
    name \
  ),

#define NAME_IMPL(s, i, type, name, data, size) \
  CHAOS_PP_IF(CHAOS_PP_EQUAL(CHAOS_PP_DEC(size), i)) ( \
    NAME(s, i, type, name, data), \
    NAME_COMMA(s, i, type, name, data) \
  )

#define CONSTRUCTOR_ARGS(i, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    NAME_IMPL, GROUP_VARIADIC(__VA_ARGS__), i, CHAOS_PP_DIV(CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__), 2) \
  ))

#define WITH_SINGLE_FUNCTION(s, i, type, name, record, ...) \
  record with_##name(const type name##_) const { \
    return { CONSTRUCTOR_ARGS(i, __VA_ARGS__) }; \
  }

#define WITH_FUNCTIONS(record, ...) \
  CHAOS_PP_EXPR(CHAOS_PP_SEQ_FOR_EACH_I( \
    WITH_SINGLE_FUNCTION, GROUP_VARIADIC(__VA_ARGS__), record, __VA_ARGS__ \
  ))