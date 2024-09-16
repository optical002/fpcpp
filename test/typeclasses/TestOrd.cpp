#include "TestOrd.h"
#include <gtest/gtest.h>

namespace typeclasses {
  TEST(Ord, PartialSpecialization) {
    TestWithOrd a, b;
    Compare(a, b);
  }

  TEST(Ord, Functionality) {
    int a = 1, b = 2;
    EXPECT_EQ(Compare(a, b), -1);
    b = 1;
    EXPECT_EQ(Compare(a, b), 0);
    b = 0;
    EXPECT_EQ(Compare(a, b), 1);
  }
}
