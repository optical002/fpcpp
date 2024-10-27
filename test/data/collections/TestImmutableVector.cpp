#include <core/data/collections/ImmutableVector.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Option.h>
#include <gtest/gtest.h>

TEST(Data_ImmutableVector, Construction) {
  const auto vector = ImmVector(1, 2, 3);
  EXPECT_EQ(vector.size(), 3);
}

TEST(Data_ImmutableVector, Empty) {
  const auto vector = EmptyImmVector<int>();
  EXPECT_TRUE(vector.isEmpty());
}

TEST(Data_ImmutableVector, ForEach) {
  const auto vector = ImmVector(1, 2, 3);
  int value = 1;
  for (int element : vector) {
    EXPECT_EQ(value, element);
    value++;
  }
}

TEST(Data_ImmutableVector, IndexOperator) {
  const auto vector = ImmVector(1, 2, 3);

  EXPECT_TRUE(Equal(vector[0], Some(1)));
  EXPECT_TRUE(Equal(vector[1], Some(2)));
  EXPECT_TRUE(Equal(vector[2], Some(3)));
  EXPECT_TRUE(Equal(vector[3], NoneOf<int>()));

  EXPECT_TRUE(Equal(vector.at(0), Some(1)));
  EXPECT_TRUE(Equal(vector.at(1), Some(2)));
  EXPECT_TRUE(Equal(vector.at(2), Some(3)));
  EXPECT_TRUE(Equal(vector.at(3), NoneOf<int>()));

  EXPECT_EQ(vector._unsafe_at(0), 1);
  EXPECT_EQ(vector._unsafe_at(1), 2);
  EXPECT_EQ(vector._unsafe_at(2), 3);
}