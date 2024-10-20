#include <gtest/gtest.h>
#include <core/data/Tag.h>
#include <core/data/ImmutableArray.h>
#include <core/data/Option.h>
#include <core/typeclasses/Semigroup.h>

template<HasContainerTagging _> void testContainer() { }

void containerCompilationTest() {
  testContainer<ImmutableArray<int, 3>>();
  testContainer<Option<int>>();
  testContainer<Either<int, int>>();
}

TEST(Data_Tag, Tag) {
  EXPECT_TRUE(Equal(Tag<AndTag>(1), Tagged<int, AndTag>(1)));
}

TEST(Data_Tag, PerformContainerTag) {
  const auto array1 = ImmArray(true, false, true, false), array2 = ImmArray(true, true, false, false);
  const auto combined = PerformContainerTag<AndTag>(
    array1, array2, [](auto a, auto b) { return Combine(a, b); }
  );
  EXPECT_TRUE(Equal(combined, ImmArray(true, false, false, false)));
}

TEST(Data_Tag, PerformTag) {
  const auto combined1 = PerformTag<AndTag>(true, false, [](auto a, auto b) { return Combine(a, b);});
  const auto combined2 = PerformTag<OrTag>(true, false, [](auto a, auto b) { return Combine(a, b);});
  EXPECT_EQ(combined1, false);
  EXPECT_EQ(combined2, true);
}

TEST(Data_Tag, Default) {
  const auto defaultInt = Default<Tagged<int, AndTag>>();
  EXPECT_EQ(defaultInt, 0);
}

TEST(Data_Tag, Eq) {
  EXPECT_TRUE(Equal(Tag<AndTag>(1), Tag<AndTag>(1)));
}

TEST(Data_Tag, Num) {
  EXPECT_TRUE(Equal(Num<Tagged<int, AndTag>>::add(Tag<AndTag>(1), Tag<AndTag>(1)), Tag<AndTag>(2)));
  EXPECT_TRUE(Equal(Num<Tagged<int, AndTag>>::subtract(Tag<AndTag>(1), Tag<AndTag>(1)), Tag<AndTag>(0)));
  EXPECT_TRUE(Equal(Num<Tagged<int, AndTag>>::multiply(Tag<AndTag>(1), Tag<AndTag>(1)), Tag<AndTag>(1)));
  EXPECT_TRUE(Equal(Num<Tagged<int, AndTag>>::divide(Tag<AndTag>(1), Tag<AndTag>(1)), Tag<AndTag>(1)));
  EXPECT_TRUE(Equal(Num<Tagged<int, AndTag>>::mod(Tag<AndTag>(1), Tag<AndTag>(1)), Tag<AndTag>(0)));
}

TEST(Data_Tag, Ord) {
  EXPECT_EQ(Compare(Tag<AndTag>(1), Tag<AndTag>(1)), 0);
}

TEST(Data_Tag, Semigroup) {
  EXPECT_EQ(Combine(Tag<AndTag>(1), Tag<AndTag>(1)), Tag<AndTag>(2));
}

TEST(Data_Tag, Container_CombineTag) {
  const auto array1 = ImmArray(true, false, true, false), array2 = ImmArray(true, true, false, false);
  EXPECT_TRUE(Equal(CombineTag<AndTag>(array1, array2), ImmArray(true, false, false, false)));
  EXPECT_TRUE(Equal(CombineTag<OrTag>(array1, array2), ImmArray(true, true, true, false)));
}

TEST(Data_Tag, ImmutableArray_Tagging) {
  const auto taggedArray = ImmArray(1, 2, 3).tag<AndTag>();
  EXPECT_TRUE(Equal(taggedArray, ImmArray(Tag<AndTag>(1), Tag<AndTag>(2), Tag<AndTag>(3))));

  const auto untaggedArray = taggedArray.unTag();
  EXPECT_TRUE(Equal(untaggedArray, ImmArray(1, 2, 3)));
}

TEST(Data_Tag, Option_Tagging) {
  const auto taggedOption = Some(1).tag<AndTag>();
  EXPECT_TRUE(Equal(taggedOption, Some(Tag<AndTag>(1))));

  const auto untaggedOption = taggedOption.unTag();
  EXPECT_TRUE(Equal(untaggedOption, Some(1)));
}

TEST(Data_Tag, Either_Tagging) {
  const auto taggedEither = RightE<std::string>(1).tag<AndTag>();
  EXPECT_TRUE(Equal(taggedEither, RightE<std::string>(Tag<AndTag>(1))));

  const auto untaggedEither = taggedEither.unTag();
  EXPECT_TRUE(Equal(untaggedEither, RightE<std::string>(1)));
}