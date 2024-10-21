#include <core/data/Option.h>
#include <core/syntax/ForComprehension.h>
#include <core/typeclasses/Eq.h>
#include <core/typeclasses/Str.h>
#include <core/typeclasses/DebugStr.h>
#include <gtest/gtest.h>

TEST(Data_Option, ForComprehension) {
  const auto comprehended = ForComprehension(Some(1),
    [](const int& x) { return Some(std::to_string(x)); },
    [](std::string x) { return Some(x.append(std::string("2"))); },
    [](std::string x) { return Some(x.append(std::string("2"))); },
    [](const std::string& x) { return std::stoi(x) + 1; }
  );
  comprehended.ifSome([](const int& x) {
    EXPECT_EQ(x, 123) << "Expected to be '123', but it was not.";
  });
}

TEST(Data_Option, Map) {
  const auto someOpt = Some(1);
  const Option<int> noneOpt = None;

  const auto mappedSomeOpt = someOpt.map([](int x) { return std::to_string(x); });
  EXPECT_TRUE(mappedSomeOpt.isSome()) << "Expected mappedSomeOpt to be 'Some', but it was not.";
  mappedSomeOpt.ifSome([](const std::string& x) {
    EXPECT_EQ(x, "1") << "Expected mappedSomeOpt to be ""1"", but it was not.";
  });

  const auto mappedNoneOpt = noneOpt.map([](int x) { return std::to_string(x); });
  EXPECT_TRUE(mappedNoneOpt.isNone()) << "Expected mappedNoneOpt to be 'None', but it was not.";
}

TEST(Data_Option, FlatMap) {
  const auto someOpt = Some(1);
  const Option<int> noneOpt = None;

  const auto flatMappedSomeOpt = someOpt.flatMap([](int x) { return Some(std::to_string(x)); });
  EXPECT_TRUE(flatMappedSomeOpt.isSome()) << "Expected flatMappedSomeOpt to be 'Some', but it was not.";
  flatMappedSomeOpt.ifSome([](const std::string& x) {
    EXPECT_EQ(x, "1") << "Expected flatMappedSomeOpt to be ""1"", but it was not.";
  });

  const auto flatMappedSomeOpt2 = someOpt.flatMap([](int x) { return None.as<std::string>(); });
  EXPECT_TRUE(flatMappedSomeOpt2.isNone()) << "Expected flatMappedSomeOpt2 to be 'None', but it was not.";

  const auto flatMappedNoneOpt = noneOpt.flatMap([](int x) { return Some(std::to_string(x)); });
  EXPECT_TRUE(flatMappedNoneOpt.isNone()) << "Expected flatMappedNoneOpt to be 'None', but it was not.";

  const auto flatMappedNoneOpt2 = noneOpt.flatMap([](int x) { return None.as<std::string>(); });
  EXPECT_TRUE(flatMappedNoneOpt2.isNone()) << "Expected flatMappedNoneOpt2 to be 'None', but it was not.";
}

TEST(Data_Option, IfSome) {
  const auto someOpt = Some(1);
  const Option<int> noneOpt = None;

  int value = 0;
  someOpt.ifSome([&value](const int& x) { value = x; });
  EXPECT_EQ(value, 1) << "Expected value to be 1, but it was not.";

  value = 2;
  noneOpt.ifSome([&value](int _) { value = 3; });
  EXPECT_EQ(value, 2) << "Expected value to be 2, but it was not.";
}

TEST(Data_Option, VoidFold) {
  const auto someOpt = Some(2);
  const Option<int> noneOpt = None;

  int value = 0;
  someOpt.voidFold(
    [&value] { value = 1; },
    [&value](const int& x) { value = x; }
  );
  EXPECT_EQ(value, 2) << "Expected value to be 2, but it was not.";

  noneOpt.voidFold(
    [&value] { value = 3; },
    [&value](int _) { value = 4; }
  );
  EXPECT_EQ(value, 3) << "Expected value to be 3, but it was not.";
}

TEST(Data_Option, Fold) {
  const auto someOpt = Some(1);
  const Option<int> noneOpt = None;

  const auto someFolded = someOpt.fold(
    0,
    [](const int x) { return x; }
  );
  EXPECT_EQ(someFolded, 1) << "Expected someFolded to be 3, but it was not.";

  const auto noneFolded = noneOpt.fold(
    0,
    [](const int x) { return x; }
  );
  EXPECT_EQ(noneFolded, 0) << "Expected noneFolded to be 3, but it was not.";
}

TEST(Data_Option, ToLeft) {
  auto someOpt = Some(std::string("x"));
  const auto leftEither = someOpt.toLeft(1);
  EXPECT_TRUE(leftEither.isLeft()) << "Expected leftEither to be 'Left', but it was not.";
}

TEST(Data_Option, ToRight) {
  auto someOpt = Some(1);
  const auto rightEither = someOpt.toRight(std::string("x"));
  EXPECT_TRUE(rightEither.isRight()) << "Expected rightEither to be 'Right', but it was not.";
}

TEST(Data_Option, Eq) {
  Option<int> none1 = None, none2 = None;

  EXPECT_TRUE(Equal(none1, none2)) << "Expected 'none1' and 'none2' to be equal";

  Option<int> some1 = Some(1), some2 = Some(2), some3 = Some(1);

  EXPECT_TRUE(Equal(some1, some3)) << "Expected 'some1' and 'some3' to be equal";
  EXPECT_FALSE(Equal(some1, some2)) << "Expected 'some1' and 'some2' to be not equal";
  EXPECT_FALSE(Equal(some1, none1)) << "Expected 'some1' and 'none1' to be not equal";
}

TEST(Data_Option, Str) {
  Option<int> none = None, some = Some(1);

  EXPECT_EQ(ToStr(none), "None");
  EXPECT_EQ(ToStr(some), "Some(1)");
}

TEST(Data_Option, DebugStr) {
  Option<int> none = None, some = Some(1);

  EXPECT_EQ(ToDebugStr(none), "None");
  EXPECT_EQ(ToDebugStr(some), "Some(Int(1))");
}

TEST(Data_Option, Flatten) {
  const auto someSome = Some(Some(1));
  const auto someNone = Some(Option<int>::none());

  const auto someSomeFlattened = someSome.flatten();
  const auto someNoneFlattened = someNone.flatten();

  EXPECT_TRUE(someSomeFlattened.isSome());
  EXPECT_FALSE(someNoneFlattened.isSome());
}

