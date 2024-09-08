// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros
#pragma once

// For each with 1 argument
#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define FOR_EACH_CON(macro, con, ...) __VA_OPT__(EXPAND(FOR_EACH_CON_HELPER(macro, con, __VA_ARGS__)))
#define FOR_EACH_CON_HELPER(macro, con, a1, ...) macro(a1)__VA_OPT__(con) __VA_OPT__(FOR_EACH_CON_AGAIN PARENS (macro, con, __VA_ARGS__))
#define FOR_EACH_CON_AGAIN() FOR_EACH_CON_HELPER

#define FOR_EACH_COMMA(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_COMMA_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_COMMA_HELPER(macro, a1, ...) macro(a1)__VA_OPT__(,) __VA_OPT__(FOR_EACH_COMMA_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_COMMA_AGAIN() FOR_EACH_COMMA_HELPER

// For each with 2 arguments
#define FOR_EACH2(macro, ...) __VA_OPT__(EXPAND(FOR_EACH2_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH2_HELPER(macro, a1, a2, ...) macro(a1, a2) __VA_OPT__(FOR_EACH2_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH2_AGAIN() FOR_EACH2_HELPER

#define FOR_EACH2_CON(macro, con, ...) __VA_OPT__(EXPAND(FOR_EACH2_CON_HELPER(macro, con, __VA_ARGS__)))
#define FOR_EACH2_CON_HELPER(macro, con, a1, a2, ...) macro(a1, a2)__VA_OPT__(con) __VA_OPT__(FOR_EACH2_CON_AGAIN PARENS (macro, con, __VA_ARGS__))
#define FOR_EACH2_CON_AGAIN() FOR_EACH2_CON_HELPER

#define FOR_EACH2_COMMA(macro, ...) __VA_OPT__(EXPAND(FOR_EACH2_COMMA_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH2_COMMA_HELPER(macro, a1, a2, ...) macro(a1, a2)__VA_OPT__(,) __VA_OPT__(FOR_EACH2_COMMA_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH2_COMMA_AGAIN() FOR_EACH2_COMMA_HELPER

// For each with 3 arguments
#define FOR_EACH3(macro, ...) __VA_OPT__(EXPAND(FOR_EACH3_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH3_HELPER(macro, a1, a2, a3, ...) macro(a1, a2, a3) __VA_OPT__(FOR_EACH3_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH3_AGAIN() FOR_EACH3_HELPER

#define FOR_EACH3_CON(macro, con, ...) __VA_OPT__(EXPAND(FOR_EACH3_CON_HELPER(macro, con, __VA_ARGS__)))
#define FOR_EACH3_CON_HELPER(macro, con, a1, a2, a3, ...) macro(a1, a2, a3)__VA_OPT__(con) __VA_OPT__(FOR_EACH3_CON_AGAIN PARENS (macro, con, __VA_ARGS__))
#define FOR_EACH3_CON_AGAIN() FOR_EACH3_CON_HELPER

#define FOR_EACH3_COMMA(macro, ...) __VA_OPT__(EXPAND(FOR_EACH3_COMMA_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH3_COMMA_HELPER(macro, a1, a2, a3, ...) macro(a1, a2, a3)__VA_OPT__(,) __VA_OPT__(FOR_EACH3_COMMA_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH3_COMMA_AGAIN() FOR_EACH3_COMMA_HELPER

// For each with 4 arguments
#define FOR_EACH4(macro, ...) __VA_OPT__(EXPAND(FOR_EACH4_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH4_HELPER(macro, a1, a2, a3, a4, ...) macro(a1, a2, a3, a4) __VA_OPT__(FOR_EACH4_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH4_AGAIN() FOR_EACH4_HELPER

#define FOR_EACH4_CON(macro, con, ...) __VA_OPT__(EXPAND(FOR_EACH4_CON_HELPER(macro, con, __VA_ARGS__)))
#define FOR_EACH4_CON_HELPER(macro, con, a1, a2, a3, a4, ...) macro(a1, a2, a3, a4)__VA_OPT__(con) __VA_OPT__(FOR_EACH4_CON_AGAIN PARENS (macro, con, __VA_ARGS__))
#define FOR_EACH4_CON_AGAIN() FOR_EACH4_CON_HELPER

#define FOR_EACH4_COMMA(macro, ...) __VA_OPT__(EXPAND(FOR_EACH4_COMMA_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH4_COMMA_HELPER(macro, a1, a2, a3, a4, ...) macro(a1, a2, a3, a4)__VA_OPT__(,) __VA_OPT__(FOR_EACH4_COMMA_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH4_COMMA_AGAIN() FOR_EACH4_COMMA_HELPER