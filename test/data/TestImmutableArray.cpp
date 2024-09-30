#include <core/data/ImmutableArray.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Option.h>
#include <gtest/gtest.h>

TEST(Data_ImmutableArray, Construction) {
  const auto array = ImmArray(1, 2, 3, 4);
  EXPECT_EQ(array.size(), 4);
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

  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 3);

  EXPECT_EQ(array.at(0), 1);
  EXPECT_EQ(array.at(1), 2);
  EXPECT_EQ(array.at(2), 3);

  EXPECT_EQ(array.at<0>(), 1);
  EXPECT_EQ(array.at<1>(), 2);
  EXPECT_EQ(array.at<2>(), 3);
}

TEST(Data_ImmutableArray, Eq) {
  const auto array1 = ImmArray(1, 2, 3), array2 = ImmArray(1, 2, 3), array3 = ImmArray(1, 2, 4);

  EXPECT_TRUE(Equal(array1, array2));
  EXPECT_FALSE(Equal(array1, array3));
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