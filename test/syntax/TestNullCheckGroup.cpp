#include <core/syntax/NullCheckGroup.h>
#include <gtest/gtest.h>

struct A { };

TEST(Syntax_NullCheckGroup, Group2) {
  A* a1 = nullptr;
  A* b1 = nullptr;

  const auto result1 = NullCheckGroup(a1, b1);
  EXPECT_TRUE(result1.isNone());

  auto aData = A{};
  A* a2 = &aData;
  A* b2 = nullptr;

  const auto result2 = NullCheckGroup(a2, b2);
  EXPECT_TRUE(result2.isNone());

  A* a3 = nullptr;
  A* b3 = &aData;

  const auto result3 = NullCheckGroup(a3, b3);
  EXPECT_TRUE(result3.isNone());

  A* a4 = &aData;
  A* b4 = &aData;

  const auto result4 = NullCheckGroup(a4, b4);
  EXPECT_TRUE(result4.isSome());
}

TEST(Syntax_NullCheckGroup, Group3) {
  A* a1 = nullptr;
  A* b1 = nullptr;
  A* c1 = nullptr;

  const auto result1 = NullCheckGroup(a1, b1, c1);
  EXPECT_TRUE(result1.isNone());

  auto aData = A{};
  A* a2 = &aData;
  A* b2 = &aData;
  A* c2 = &aData;

  const auto result4 = NullCheckGroup(a2, b2, c2);
  EXPECT_TRUE(result4.isSome());
}

TEST(Syntax_NullCheckGroup, Group4) {
  A* a1 = nullptr;
  A* b1 = nullptr;
  A* c1 = nullptr;
  A* d1 = nullptr;

  const auto result1 = NullCheckGroup(a1, b1, c1, d1);
  EXPECT_TRUE(result1.isNone());

  auto aData = A{};
  A* a2 = &aData;
  A* b2 = &aData;
  A* c2 = &aData;
  A* d2 = &aData;

  const auto result4 = NullCheckGroup(a2, b2, c2, d2);
  EXPECT_TRUE(result4.isSome());
}
