#ifndef FPCPP_TEST_TYPECLASSES_TEST_ORD_H
#define FPCPP_TEST_TYPECLASSES_TEST_ORD_H

#include <core/typeclasses/Ord.h>

struct TestWithOrd {};

template<>
struct Ord<TestWithOrd> {
  static int compare(const TestWithOrd& a, const TestWithOrd& b) {
    return 0;
  }
};

#endif // FPCPP_TEST_TYPECLASSES_TEST_ORD_H