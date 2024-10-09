#include "TestSemigroup.h"
#include <gtest/gtest.h>

TEST(Typeclasses_Semigroup, PartialSpecialization) {
  TestWithSemigroup a, b;
  Combine(a, b);
}

TEST(Typeclasses_Semigroup, Functionality) {
  std::string a = "hello", b = " world";
  EXPECT_EQ("hello world", Combine(a, b));
}

TEST(Typeclasses_Semigroup, Tagging) {
  EXPECT_EQ(CombineTag<AndTag>(true, true), true);
  EXPECT_EQ(CombineTag<AndTag>(true, false), false);
  EXPECT_EQ(CombineTag<OrTag>(true, false), true);
}
