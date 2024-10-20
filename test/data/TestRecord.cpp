#include "TestRecord.h"
#include <gtest/gtest.h>

TEST(Data_Record, Equality) {
  TestNoParams x1, x2;
  EXPECT_TRUE(x1 == x2) << "Expected x1 to be equal x2, but it was not.";

  TestWithParams x3 = TestWithParams(1, 1.f, "hello", TestNoParams());
  TestWithParams x4 = TestWithParams(1, 1.f, "hello", TestNoParams());
  EXPECT_TRUE(x3 == x4) << "Expected x3 to be equal x4, but it was not.";

  TestWithParams x5 = TestWithParams(2, 1.1f, "hello", TestNoParams());
  EXPECT_TRUE(x3 != x5) << "Expected x3 to be not equal x5, but it was.";
}

TEST(Data_Record, ToString) {
  TestNoParams x1;
  EXPECT_EQ(ToStr(x1), "TestNoParams()");

  TestWithParams x2 = TestWithParams(1, 1.f, "hello", TestNoParams());
  EXPECT_EQ(ToStr(x2), "TestWithParams(age=Int(1), cost=Float(1), name=String(hello), testNoParams=TestNoParams())");
}

TEST(Data_Record, WithConstructors) {
  TestWithParams x = TestWithParams(1, 1.f, "hello", TestNoParams());
  const auto xAge = x.with_age(2);
  EXPECT_EQ(x.age(), 1) << "Expected x.age to still be 1, but it was not.";
  EXPECT_EQ(xAge.age(), 2) << "Expected xAge.age to be 2, but it was not.";

  const auto xCost = x.with_cost(2.f);
  EXPECT_EQ(x.cost(), 1.f) << "Expected x.cost to still be 1.f, but it was not.";
  EXPECT_EQ(xCost.cost(), 2.f) << "Expected xCost.cost to be 2.f, but it was not.";

  const auto xName = x.with_name("world");
  EXPECT_EQ(x.name(), "hello") << "Expected x.name to be 'hello', but it was not.";
  EXPECT_EQ(xName.name(), "world") << "Expected xName.name to be 'world', but it was not.";
}
