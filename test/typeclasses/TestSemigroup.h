#pragma once
#include "core/typeclasses/Semigroup.h"

class TestWithSemigroup { };

template<>
struct Semigroup<TestWithSemigroup> {
  static TestWithSemigroup combine(const TestWithSemigroup& a, const TestWithSemigroup& b) {
    return a;
  }
};
