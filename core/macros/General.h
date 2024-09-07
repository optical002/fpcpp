// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros
// ReSharper disable CppClangTidyModernizeMacroToEnum
#pragma once

#define CONST_SINGLE_FIELD(type, name) const type name;
#define CONST_FIELDS(...) FOR_EACH2(CONST_SINGLE_FIELD, __VA_ARGS__)

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define EMPTY()
#define DEFER(id) id EMPTY()

#define STRINGIFY(x) #x