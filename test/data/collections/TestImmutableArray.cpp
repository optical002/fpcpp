#include <core/data/collections/ImmutableArray.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Option.h>
#include <gtest/gtest.h>

TEST(Data_ImmutableArray, Construction) {
  const auto array = ImmArray(1, 2, 3, 4);
  EXPECT_EQ(array.size(), 4);
}

TEST(Data_ImmutableArray, Empty) {
  const auto emptyArr = EmptyImmArray<int>();
  EXPECT_TRUE(emptyArr.isEmpty());
}

TEST(Data_ImmutableArray, ForEach) {
  const auto array = ImmArray(1, 2, 3);
  int value = 1;
  for (int element : array) {
    EXPECT_EQ(value, element);
    value++;
  }
}

TEST(Data_ImmutableArray, IndexOperator) {
  const auto array = ImmArray(1, 2, 3);

  EXPECT_TRUE(Equal(array[0], Some(1)));
  EXPECT_TRUE(Equal(array[1], Some(2)));
  EXPECT_TRUE(Equal(array[2], Some(3)));
  EXPECT_TRUE(Equal(array[3], NoneOf<int>()));

  EXPECT_TRUE(Equal(array.at(0), Some(1)));
  EXPECT_TRUE(Equal(array.at(1), Some(2)));
  EXPECT_TRUE(Equal(array.at(2), Some(3)));
  EXPECT_TRUE(Equal(array.at(3), NoneOf<int>()));

  EXPECT_EQ(array._unsafe_at(0), 1);
  EXPECT_EQ(array._unsafe_at(1), 2);
  EXPECT_EQ(array._unsafe_at(2), 3);

  EXPECT_EQ(array.at<0>(), 1);
  EXPECT_EQ(array.at<1>(), 2);
  EXPECT_EQ(array.at<2>(), 3);
}

TEST(Data_ImmutableArray, Map) {
  const auto array = ImmArray(1, 2, 3);
  const auto mapped = array.map([](auto x) {return std::to_string(x);});

  int value = 1;
  for (const std::string& element : mapped) {
    EXPECT_EQ(std::to_string(value), element);
    value++;
  }
}

TEST(Data_ImmutableArray, FlatMap) {
  const auto array = ImmArray(1, 2, 3);
  const auto flatMapped = array.flatMap([](int x) {return ImmArray(x, x);});
  EXPECT_TRUE(Equal(flatMapped, ImmArray(1, 1, 2, 2, 3, 3)));
}

TEST(Data_ImmutableArray, Flatten) {
  const auto array = ImmArray(ImmArray(1, 2), ImmArray(3, 4), ImmArray(5, 6));
  const auto flattened = array.flatten();
  int value = 1;
  for (int element : flattened) {
    EXPECT_EQ(value, element);
    value++;
  }
}

TEST(Data_ImmutableArray, Reduce) {
  const auto array = ImmArray(1, 2, 3);
  const auto reduced = array.reduce([](double a, int b) {return a + b;});

  EXPECT_EQ(reduced, 6.);
}

TEST(Data_ImmutableArray, Fold) {
  const auto array = ImmArray(1, 2, 3);
  const auto reduced = array.fold(std::string("0"), [](std::string a, int b) {
    return std::format("{} {}", a, std::to_string(b));
  });

  EXPECT_EQ(reduced, "0 1 2 3");
}

TEST(Data_ImmutableArray, Order) {
  const auto array = ImmArray(4, 5, 3, 6);
  const auto orderedAscending = array.order();
  const auto orderedDescending = array.order(Order::Descending);
  EXPECT_TRUE(Equal(array, ImmArray(4, 5, 3, 6)));
  EXPECT_TRUE(Equal(orderedAscending, ImmArray(3, 4, 5, 6)));
  EXPECT_TRUE(Equal(orderedDescending, ImmArray(6, 5, 4, 3)));
}

struct Data {
  int value;
};

template<>
struct Eq<Data> {
  static bool equal(const Data& a, const Data& b) {
    return a.value == b.value;
  }
};


TEST(Data_ImmutableArray, OrderBy) {
  const auto array = ImmArray(Data(9), Data(5), Data(7), Data(2));
  const auto orderedAscending = array.orderBy(
    [](Data data) { return data.value; }
  );
  const auto orderedDescending = array.orderBy(
    [](Data data) { return data.value; }, Order::Descending
  );
  EXPECT_TRUE(Equal(array, ImmArray(Data(9), Data(5), Data(7), Data(2))));
  EXPECT_TRUE(Equal(orderedAscending, ImmArray(Data(2), Data(5), Data(7), Data(9))));
  EXPECT_TRUE(Equal(orderedDescending, ImmArray(Data(9), Data(7), Data(5), Data(2))));
}

TEST(Data_ImmutableArray, Head) {
  const auto array = ImmArray(1, 2, 3);
  EXPECT_EQ(array.head(), 1);
}

TEST(Data_ImmutableArray, Tail) {
  const auto array = ImmArray(1, 2, 3);
  EXPECT_EQ(array.tail(), 3);
}

TEST(Data_ImmutableArray, Find) {
  const auto array = ImmArray(1, 2, 3);
  const auto found = array.find([](int x) {return x == 2;});
  EXPECT_TRUE(found.isSome());
  found.ifSome([](int x) {EXPECT_EQ(x, 2);});

  const auto notFound = array.find([](int x) {return x == 4;});
  EXPECT_TRUE(notFound.isNone());
}

TEST(Data_ImmutableArray, Exists) {
  const auto array = ImmArray(1, 2, 3);
  EXPECT_TRUE(array.exists([](int x) { return x == 2; }));
  EXPECT_FALSE(array.exists([](int x) { return x == 4; }));
}

TEST(Data_ImmutableArray, ForAll) {
  const auto array = ImmArray(1, 2, 3);
  EXPECT_TRUE(array.forall([](int x) { return x == 1 || x == 2 || x == 3; }));
  EXPECT_FALSE(array.forall([](int x) { return x == 2; }));
}

TEST(Data_ImmutableArray, Take) {
  const auto array = ImmArray(1, 2, 3, 4, 5);
  const auto i = 3;
  const auto taken = array.take<i>();
  EXPECT_TRUE(Equal(array, ImmArray(1, 2, 3, 4, 5)));
  EXPECT_TRUE(Equal(taken, ImmArray(1, 2, 3)));
}

TEST(Data_ImmutableArray, Drop) {
  const auto array = ImmArray(1, 2, 3, 4, 5);
  const auto i = 3;
  const auto dropped = array.drop<i>();
  EXPECT_TRUE(Equal(array, ImmArray(1, 2, 3, 4, 5)));
  EXPECT_TRUE(Equal(dropped, ImmArray(4, 5)));
}

TEST(Data_ImmutableArray, Reverse) {
  const auto array = ImmArray(1, 2, 3);
  const auto reversed = array.reverse();
  EXPECT_TRUE(Equal(array, ImmArray(1, 2, 3)));
  EXPECT_TRUE(Equal(reversed, ImmArray(3, 2, 1)));
}

TEST(Data_ImmutableArray, MkStringToStr) {
  const auto array = ImmArray(1, 2, 3);
  const auto str = array.mkStringToStr();
  EXPECT_EQ(str, "1, 2, 3");
}

struct Data2 {
  int value;
};

TEST(Data_ImmutableArray, MkString) {
  const auto array = ImmArray(Data2(1), Data2(2), Data2(3));
  const auto str = array.mkString([](Data2 x) { return std::to_string(x.value); });
  EXPECT_EQ(str, "1, 2, 3");
}

TEST(Data_ImmutableArray, Sum) {
  const auto array = ImmArray(1, 2, 3);
  EXPECT_EQ(array.sum(), 6);
}

TEST(Data_ImmutableArray, Combine) {
  const auto array = ImmArray("1", "2");
  EXPECT_EQ(array.combine(), "12");

  const auto array2 = ImmArray(1, 2);
  EXPECT_EQ(array2.combine(), 3);
}

TEST(Data_ImmutableArray, ZipWith) {
  const auto array = ImmArray(1, 2, 3);
  const auto array2 = ImmArray(4, 5, 6, 7);
  const auto zipped = array.zipWith(array2, [](int x, int y) { return x + y; });
  EXPECT_TRUE(Equal(zipped, ImmArray(5, 7, 9)));

  const auto zipped2 = array.zipWith(array2);
  EXPECT_TRUE(Equal(zipped2, ImmArray(Tpl(1, 4), Tpl(2, 5), Tpl(3, 6))));
}

TEST(Data_ImmutableArray, ZipWithAll) {
  const auto array = ImmArray(1, 2, 3);
  const auto array2 = ImmArray(4, 5, 6, 7);
  const auto zipped = array.zipWithAll(
    array2, [](int x, int y) { return x + y; }, 1
  );
  EXPECT_TRUE(Equal(zipped, ImmArray(5, 7, 9, 8)));

  const auto zipped2 = array.zipWithAll(array2, 1);
  EXPECT_TRUE(Equal(zipped2, ImmArray(Tpl(1, 4), Tpl(2, 5), Tpl(3, 6), Tpl(1, 7))));
}

TEST(Data_ImmutableArray, ZipWithIndex) {
  const auto array = ImmArray("a", "b", "c");
  const auto arrayWithIdx = array.zipWithIndex();
  int i = 0;
  for (const auto& x : arrayWithIdx) {
    EXPECT_EQ(x, Tpl(array._unsafe_at(i), i));
    i++;
  }
}

TEST(Data_ImmutableArray, Slice) {
  const auto array = ImmArray(4, 5, 6, 7);
  EXPECT_TRUE(Equal(array.slice<0, 2>(), ImmArray(4, 5, 6)));
  EXPECT_TRUE(Equal(array.slice<0, 0>(), ImmArray(4)));
  EXPECT_TRUE(Equal(array.slice<0, 3>(), ImmArray(4, 5, 6, 7)));
  EXPECT_TRUE(Equal(array.slice<2, 3>(), ImmArray(6, 7)));
}

TEST(Data_ImmutableArray, Eq) {
  const auto array1 = ImmArray(1, 2, 3), array2 = ImmArray(1, 2, 3), array3 = ImmArray(1, 2, 4);

  EXPECT_TRUE(Equal(array1, array2));
  EXPECT_FALSE(Equal(array1, array3));
}

TEST(Data_ImmutableArray, Default) {
  EXPECT_TRUE(Equal(EmptyImmArray<int>(), Default<ImmutableArray<int, 0>>()));
}

TEST(Data_ImmutableArray, Semigroup) {
  const auto array1 = ImmArray(1, 2, 3, 4);
  const auto array2 = ImmArray(5, 6, 7, 8);
  EXPECT_TRUE(Equal(Combine(array1, array2), ImmArray(6, 8, 10, 12)));
}

TEST(Data_ImmutableArray, Str) {
  const auto array = ImmArray(1, 2, 3);
  EXPECT_EQ(ToStr(array), "ImmutableArray(1, 2, 3)");
}

TEST(Data_ImmutableArray, DebugStr) {
  const auto array = ImmArray(1, 2, 3);
  EXPECT_EQ(ToDebugStr(array), "ImmutableArray[3]([0]=Int(1), [1]=Int(2), [2]=Int(3))");
}