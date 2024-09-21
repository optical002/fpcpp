#ifndef FPCPP_TEST_TYPECLASSES_TEST_NUM_H
#define FPCPP_TEST_TYPECLASSES_TEST_NUM_H

#include <core/typeclasses/Num.h>

class TestWithNum {};

template<>
struct Num<TestWithNum> {
  static TestWithNum add(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum subtract(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum multiply(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum divide(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum mod(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
};

#endif // FPCPP_TEST_TYPECLASSES_TEST_NUM_H