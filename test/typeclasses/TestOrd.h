#pragma once
#include <core/typeclasses/Ord.h>

struct TestWithOrd {};

template<>
struct Ord<TestWithOrd> {
  static int compare(const TestWithOrd& a, const TestWithOrd& b) {
    return 0;
  }
};
