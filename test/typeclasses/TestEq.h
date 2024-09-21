#ifndef FPCPP_TEST_TYPECLASSES_TEST_EQ_H
#define FPCPP_TEST_TYPECLASSES_TEST_EQ_H

#include <core/typeclasses/Eq.h>

struct TestWithEq {};

template<>
struct Eq<TestWithEq> {
  static bool equal(const TestWithEq& a, const TestWithEq& b) {
    return true;
  }
};

#endif // FPCPP_TEST_TYPECLASSES_TEST_EQ_H