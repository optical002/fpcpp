// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros, CppClangTidyModernizeMacroToEnum
#ifndef FPCPP_CORE_MACROS_GENERAL_H
#define FPCPP_CORE_MACROS_GENERAL_H

#include <core/macros/ForEach.h>

#define SINGLE_PRIVATE_FIELD(type, name) type _##name;
#define PRIVATE_FIELDS(...) FOR_EACH2(SINGLE_PRIVATE_FIELD, __VA_ARGS__)

#define SINGLE_FIELD_GETTER(type, name) type name() const { return _##name; }
#define FIELDS_GETTERS(...) FOR_EACH2(SINGLE_FIELD_GETTER, __VA_ARGS__)

#define PRIVATE_CONSTRUCTOR_SINGLE_ARG(type, name) type name
#define PRIVATE_CONSTRUCTOR_ARGS(...) FOR_EACH2_COMMA(PRIVATE_CONSTRUCTOR_SINGLE_ARG, __VA_ARGS__)

#define PRIVATE_CONSTRUCTOR_SINGLE_SET(type, name) _##name(name)
#define PRIVATE_CONSTRUCTOR_SET(...) FOR_EACH2_COMMA(PRIVATE_CONSTRUCTOR_SINGLE_SET, __VA_ARGS__)

#define PRIVATE_CONSTRUCTOR(type, ...) \
  type( \
    PRIVATE_CONSTRUCTOR_ARGS(__VA_ARGS__) \
  ) : PRIVATE_CONSTRUCTOR_SET(__VA_ARGS__) { }

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define EMPTY()
#define DEFER(id) id EMPTY()

#define STRINGIFY(x) #x

#endif // FPCPP_CORE_MACROS_GENERAL_H