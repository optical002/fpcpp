#include "TestSemigroup.h"
#include <gtest/gtest.h>

namespace typeclasses {
  TEST(Semigroup, PartialSpecialization) {
    TestWithSemigroup a, b;
    Combine(a, b);
  }

  TEST(Semigroup, Functionality) {
    std::string a = "hello", b = " world";
    EXPECT_EQ("hello world", a + b);
  }
}
