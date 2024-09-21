#include "TestEq.h"
#include <gtest/gtest.h>

TEST(Typeclasses_Eq, PartialSpecialization) {
  TestWithEq a, b;
  Equal(a, b);
}

TEST(Typeclasses_Eq, Functionality) {
  int a = 1, b = 2;
  EXPECT_FALSE(Equal(a, b)) << "Expected a to be different from b, but it was not.";
  b = 1;
  EXPECT_TRUE(Equal(a, b)) << "Expected a to be equal to b, but it was not.";

  std::string c = "Hello", d = "World";
  EXPECT_FALSE(Equal(c, d)) << "Expected c to be different from d, but it was not.";
  d = "Hello";
  EXPECT_TRUE(Equal(c, d)) << "Expected c to be equal to d, but it was not.";
}
