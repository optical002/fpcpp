#include "TestSemigroup.h"
#include <gtest/gtest.h>

TEST(Typeclasses_Semigroup, PartialSpecialization) {
  TestWithSemigroup a, b;
  Combine(a, b);
}

TEST(Typeclasses_Semigroup, Functionality) {
  std::string a = "hello", b = " world";
  EXPECT_EQ("hello world", a + b);
}
