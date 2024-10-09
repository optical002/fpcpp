#include <core/data/Variant.h>
#include <gtest/gtest.h>

TEST(Data_Variant, Functionality) {
  const auto variant = Variant<bool, char, int, bool, double>::create<2>(69);
  const auto gotNone = variant.get<1>();
  const auto gotSome = variant.get<2>();
  const auto getUnsafe = variant._unsafe_get<2>();
  EXPECT_TRUE(Equal(gotNone, NoneOf<char>()));
  EXPECT_TRUE(Equal(gotSome, Some(69)));
  EXPECT_EQ(getUnsafe, 69);

  const auto variant2 = Variant<const char*>::create<0>("Hello");
  const auto getUnsafe2 = variant2._unsafe_get<0>();
  EXPECT_EQ(getUnsafe2, "Hello");


  const auto variant3 = Variant<std::shared_ptr<int>>::create<0>(std::make_shared<int>(79));
  const auto getUnsafe3 = variant3._unsafe_get<0>();
  EXPECT_EQ(*getUnsafe3, 79);
}

TEST(Data_Variant, Eq) {
  const auto variant1 = Variant<int, bool>::create<0>(1), variant2 = Variant<int, bool>::create<0>(1);
  EXPECT_TRUE(Equal(variant1, variant2));
}

TEST(Data_Variant, ToString) {
  const auto variant = Variant<char, int, double>::create<1>(69);
  EXPECT_EQ(ToStr(variant), "Variant(index=1, bytes=0x4500000000000000)");
}