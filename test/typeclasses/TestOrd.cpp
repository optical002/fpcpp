#include "TestOrd.h"
#include <gtest/gtest.h>

namespace typeclasses {
  TEST(Ord, PartialSpecialization) {
    TestWithOrd a, b;
    Compare(a, b);
  }

  TEST(Ord, Functionality) {
    int a = 1, b = 2;
    EXPECT_EQ(Compare(a, b), -1) << "Expected a to be less than b, but it was not.";
    b = 1;
    EXPECT_EQ(Compare(a, b), 0) << "Expected a to be equal to b, but it was not.";
    b = 0;
    EXPECT_EQ(Compare(a, b), 1) << "Expected a to be greater than b, but it was not.";
  }
}
