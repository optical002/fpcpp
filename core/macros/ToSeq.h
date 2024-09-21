// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros
#ifndef FPCPP_CORE_MACROS_TOSEQ_H
#define FPCPP_CORE_MACROS_TOSEQ_H

#include <core/macros/General.h>
#include <chaos/preprocessor/tuple/size.h>

#define TO_SEQ_1(x1) (x1)
#define TO_SEQ_2(x1, x2) (x1)(x2)
#define TO_SEQ_3(x1, x2, x3) (x1)(x2)(x3)
#define TO_SEQ_4(x1, x2, x3, x4) (x1)(x2)(x3)(x4)
#define TO_SEQ_5(x1, x2, x3, x4, x5) (x1)(x2)(x3)(x4)(x5)
#define TO_SEQ_6(x1, x2, x3, x4, x5, x6) (x1)(x2)(x3)(x4)(x5)(x6)
#define TO_SEQ_7(x1, x2, x3, x4, x5, x6, x7) (x1)(x2)(x3)(x4)(x5)(x6)(x7)
#define TO_SEQ_8(x1, x2, x3, x4, x5, x6, x7, x8) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)
#define TO_SEQ_9(x1, x2, x3, x4, x5, x6, x7, x8, x9) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)
#define TO_SEQ_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)
#define TO_SEQ_11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)
#define TO_SEQ_12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)
#define TO_SEQ_13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)
#define TO_SEQ_14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)
#define TO_SEQ_15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)
#define TO_SEQ_16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)
#define TO_SEQ_17(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)
#define TO_SEQ_18(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)
#define TO_SEQ_19(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)(x19)
#define TO_SEQ_20(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)(x19)(x20)
#define TO_SEQ_21(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)(x19)(x20)(x21)
#define TO_SEQ_22(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)(x19)(x20)(x21)(x22)
#define TO_SEQ_23(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)(x19)(x20)(x21)(x22)(x23)
#define TO_SEQ_24(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)(x19)(x20)(x21)(x22)(x23)(x24)
#define TO_SEQ_25(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, x21, x22, x23, x24, x25) (x1)(x2)(x3)(x4)(x5)(x6)(x7)(x8)(x9)(x10)(x11)(x12)(x13)(x14)(x15)(x16)(x17)(x18)(x19)(x20)(x21)(x22)(x23)(x24)(x25)

#define TO_SEQ_X(x) TO_SEQ_##x
/**
 * Converts arguments to sequence.
 * Supports up to 25 arguments.
 */
#define TO_SEQ_VARIADIC(...) EXPAND(DEFER(TO_SEQ_X) (CHAOS_PP_VARIADIC_SIZE(__VA_ARGS__)))(__VA_ARGS__)

#endif // FPCPP_CORE_MACROS_TOSEQ_H
