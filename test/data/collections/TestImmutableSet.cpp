#include <core/data/collections/ImmutableSet.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Option.h>
#include <gtest/gtest.h>

TEST(Data_ImmutableSet, Construction) {
  const auto set = ImmSet(1, 2, 3, 3);
  EXPECT_EQ(set.size(), 3);
}

TEST(Data_ImmutableSet, Empty) {
  const auto set = EmptyImmSet<int>();
  EXPECT_TRUE(set.isEmpty());
}

TEST(Data_ImmutableSet, ForEach) {
  const auto set = ImmSet(1, 2, 3);
  int value = 1;
  for (int element : set) {
    EXPECT_EQ(value, element);
    value++;
  }
}