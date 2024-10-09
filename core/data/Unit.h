#ifndef FPCPP_CORE_DATA_UNIT_H
#define FPCPP_CORE_DATA_UNIT_H

#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Eq.h>

struct Unit { };

template<>
struct ToString<Unit> {
  static std::string toStr(const Unit&) { return "Unit"; }
};

template<>
struct Eq<Unit> {
  static bool equal(const Unit&, const Unit&) { return true; }
};

#endif // FPCPP_CORE_DATA_UNIT_H
