#include <core/data/collections/ImmutableMap.h>
#include <core/data/Functions.h>
#include <gtest/gtest.h>

TEST(Data_ImmutableMap, Construction) {
  const auto kv = KV(1, "one");
  EXPECT_EQ(kv, std::make_pair(1, "one"));

  const auto map = ImmMap(KV(1, "one"), KV(2, "two"));
  EXPECT_EQ(map.size(), 2);
}

TEST(Data_ImmutableMap, Empty) {
  const auto emptyMap = EmptyImmMap<int, int>();
  EXPECT_TRUE(emptyMap.isEmpty());
}

TEST(Data_ImmutableMap, ForEach) {
  const auto map = ImmMap(KV(1, "one"), KV(2, "two"));
  int value = 1;
  for (auto kv : map) {
    EXPECT_EQ(kv.first, value);
    value++;
  }
}

TEST(Data_ImmutableMap, IndexOperator) {
  const auto map = ImmMap(KV(1, "one"), KV(2, "two"));

  EXPECT_TRUE(Equal(map[1], Some("one")));
  EXPECT_TRUE(Equal(map[2], Some("two")));
  EXPECT_TRUE(Equal(map[3], NoneOf<std::string>()));

  EXPECT_TRUE(Equal(map.at(1), Some("one")));
  EXPECT_TRUE(Equal(map.at(2), Some("two")));
  EXPECT_TRUE(Equal(map.at(3), NoneOf<std::string>()));

  EXPECT_TRUE(Equal(map._unsafe_at(1), "one"));
  EXPECT_TRUE(Equal(map._unsafe_at(2), "two"));
}

TEST(Data_ImmutableMap, Map) {
  const auto map = ImmMap(KV(1, 1), KV(2, 2));
  const auto mapped = map.map([](const auto& x) { return std::to_string(x); });

  for (auto i = 1; i <= 2; i++) {
    EXPECT_TRUE(Equal(mapped[i], Some(std::to_string(i))));
  }
}

TEST(Data_ImmutableMap, FlatMap) {
  const auto map = ImmMap(KV(1, 1), KV(2, 2));

  const auto flatMapped = map.flatMap([](const auto& x) {
    return ImmMap(KV(x, x * x));
  });

  for (auto i = 1; i <= 2; i++) {
    EXPECT_TRUE(Equal(flatMapped[i], Some(i * i)));
  }
}