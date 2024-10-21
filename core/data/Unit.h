#ifndef FPCPP_CORE_DATA_UNIT_H
#define FPCPP_CORE_DATA_UNIT_H

#include <core/typeclasses/Str.h>
#include <core/typeclasses/DebugStr.h>
#include <core/typeclasses/Eq.h>

struct Unit { };

template<>
struct Str<Unit> {
  static std::string toStr(const Unit&) { return "_"; }
};

template<>
struct DebugStr<Unit> {
  static std::string toDebugStr(const Unit&) { return "Unit"; }
};

template<>
struct Eq<Unit> {
  static bool equal(const Unit&, const Unit&) { return true; }
};

#endif // FPCPP_CORE_DATA_UNIT_H
