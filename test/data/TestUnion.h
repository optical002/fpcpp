#ifndef FPCPP_TEST_DATA_TEST_UNION_H
#define FPCPP_TEST_DATA_TEST_UNION_H

#include <core/data/Union.h>

GEN_UNION(State,
  std::string, name,
  int, age,
  float, cost,
  float, height
)

// TODO write test for it.
GEN_UNION_TEMPLATE(StateA, A,
  A, raw,
  Option<A>, maybeRaw,
  Unit, something,
  Unit, something2
)

#endif //FPCPP_TEST_DATA_TEST_UNION_H
