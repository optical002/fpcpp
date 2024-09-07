#pragma once
#include <core/typeclasses/Eq.h>

struct TestWithEq {};

template<>
struct Eq<TestWithEq> {
  static bool equal(const TestWithEq& a, const TestWithEq& b) {
    return true;
  }
};
