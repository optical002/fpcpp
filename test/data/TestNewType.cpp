#include "TestNewType.h"
#include <gtest\gtest.h>

TEST(Data_NewType, Equality) {
  const Age x1 = {.a = 1}, x2 = {.a = 1};
  EXPECT_TRUE(x1 == x2) << "Expected x1 to be equal x2, but it was not.";

  const Age x3 = {.a = 2};
  EXPECT_TRUE(x1 != x3) << "Expected x1 to be not equal x3, but it was.";
}

TEST(Data_NewType, ToString) {
  const Age x1 = {.a = 1};
  EXPECT_EQ(ToStr(x1), "Age(a=Int(1))");
}

TEST(Data_NewType, WithConstructors) {
  const Age x = {.a = 1};
  const auto xA = x.with_a(2);
  EXPECT_EQ(x.a, 1) << "Expected x.a to still be 1, but it was not.";
  EXPECT_EQ(xA.a, 2) << "Expected xA.a to be 2, but it was not.";
}

TEST(Data_NewType, SemigroupCombine) {
  const Name x1 = Name("Hello"), x2 = Name(" World");

  EXPECT_EQ(Combine(x1, x2).a, "Hello World");
  EXPECT_EQ((x1 + x2).a, "Hello World");
}

TEST(Data_NewType, Ordering) {
  const Age x1 = Age(1), x2 = Age(2);

  EXPECT_TRUE(x1 < x2) << "Expected x1 < x2";
  EXPECT_TRUE(x1 <= x2) << "Expected x1 <= x2";
  EXPECT_TRUE(x2 > x1) << "Expected x2 > x1";
  EXPECT_TRUE(x2 >= x1) << "Expected x2 >= x1";
}

TEST(Data_NewType, Num) {
  const Age x1 = Age(1), x2 = Age(2);

  EXPECT_EQ((x1 + x2).a, 3);
  EXPECT_EQ((x2 - x1).a, 1);
  EXPECT_EQ((x1 * x2).a, 2);
  EXPECT_EQ((x2 / x1).a, 2);
  EXPECT_EQ((x2 % x1).a, 0);
}