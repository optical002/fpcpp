#ifndef FPCPP_TEST_TYPECLASSES_TEST_SEMIGROUP_H
#define FPCPP_TEST_TYPECLASSES_TEST_SEMIGROUP_H

#include "core/typeclasses/Semigroup.h"

class TestWithSemigroup { };

template<>
struct Semigroup<TestWithSemigroup> {
  static TestWithSemigroup combine(const TestWithSemigroup& a, const TestWithSemigroup& b) {
    return a;
  }
};

#endif // FPCPP_TEST_TYPECLASSES_TEST_SEMIGROUP_H