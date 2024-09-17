#include "TestNewType.h"
#include <gtest\gtest.h>

namespace data {
  TEST(NewType, Equality) {
    const Age x1 = {.a = 1}, x2 = {.a = 1};
    EXPECT_TRUE(x1 == x2) << "Expected x1 to be equal x2, but it was not.";

    const Age x3 = {.a = 2};
    EXPECT_TRUE(x1 != x3) << "Expected x1 to be not equal x3, but it was.";
  }

  TEST(NewType, ToString) {
    const Age x1 = {.a = 1};
    EXPECT_EQ(ToStr(x1), "Age(a=Int(1))");
  }

  TEST(NewType, WithConstructors) {
    const Age x = {.a = 1};
    const auto xA = x.with_a(2);
    EXPECT_EQ(x.a, 1) << "Expected x.a to still be 1, but it was not.";
    EXPECT_EQ(xA.a, 2) << "Expected xA.a to be 2, but it was not.";
  }

}
