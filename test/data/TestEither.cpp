#include <core/data/Either.h>
#include <core/syntax/ForComprehension.h>
#include <gtest/gtest.h>

TEST(Data_Either, ForComprehension) {
  const auto comprehended = ForComprehension(RightE<std::string>(1),
    [](const int& x) { return RightE<std::string>(std::to_string(x)); },
    [](std::string x) { return RightE<std::string>(x.append(std::string("2"))); },
    [](std::string x) { return RightE<std::string>(x.append(std::string("2"))); },
    [](std::string x) { return std::stoi(x) + 1; }
  );
  comprehended.right().ifSome([](const int& x) {
    EXPECT_EQ(x, 123) << "Expected to be '123', but it was not.";
  });
}

TEST(Data_Either, Inferance) {
  bool b = true;
  const auto e1 = b ? Left(std::string("x")) : Right(4).to<std::string>();
  const auto e2 = b ? Right(4).to<std::string>() : Left(std::string("x"));

  const Either<std::string, int> l = LeftE<int>(std::string("a"));
  const Either<std::string, int> r = RightE<std::string>(1);
}

TEST(Data_Either, VoidFold) {
  const Either<std::string, int> leftEither = Left(std::string("x"));
  const Either<std::string, int> rightEither = Right(4);

  int value = 0;
  leftEither.voidFold(
    [&value](const std::string& _) { value = 1; },
    [&value](const int& _) { value = 2; }
  );
  EXPECT_EQ(value, 1) << "Expected value to be 1, but it was not.";

  rightEither.voidFold(
    [&value](const std::string& _) { value = 3; },
    [&value](const int& x) { value = x; }
  );
  EXPECT_EQ(value, 4) << "Expected value to be 4, but it was not.";
}

TEST(Data_Either, Fold) {
  const auto leftE = Left(std::string("a")).to<int>();
  const auto rightE = Right(1).to<std::string>();

  const auto leftFolded = leftE.fold(
    [](const std::string& str) { return str;},
    [](int _) { return std::string("b"); }
  );
  EXPECT_EQ(leftFolded, "a") << "Expected left folded value to be 'a', but it was not.";

  const auto rightFolded = rightE.fold(
    [](std::string _) { return 0; },
    [](const int& x) { return x; }
  );
  EXPECT_EQ(rightFolded, 1) << "Expected right folded value to be 1, but it was not.";
}

TEST(Data_Either, Map) {
  const auto leftE = Left(std::string("a")).to<int>();
  const auto rightE = Right(1).to<std::string>();

  const auto leftMapped = leftE.map(
    [](int _) { return std::string("b"); }
  );
  EXPECT_TRUE(leftMapped.isLeft()) << "Expected left mapped value to be left, but it was not.";

  const auto rightMapped = rightE.map(
    [](const int& x) { return std::to_string(x); }
  );
  EXPECT_TRUE(rightMapped.isRight()) << "Expected right mapped value to be right, but it was not.";
  rightMapped.right().ifSome([](const std::string& x) {
    EXPECT_EQ(x, "1") << "Expected right mapped value to be '1', but it was not.";
  });
}

TEST(Data_Either, MapLeft) {
  const auto leftE = Left(std::string("a")).to<int>();
  const auto rightE = Right(1).to<std::string>();

  const auto leftMapped = leftE.mapLeft(
    [](std::string _) { return 1; }
  );
  EXPECT_TRUE(leftMapped.isLeft()) << "Expected left mapped value to be left, but it was not.";
  leftMapped.left().ifSome([](const int& x) {
    EXPECT_EQ(x, 1) << "Expected left mapped value to be '1', but it was not.";
  });

  const auto rightMapped = rightE.mapLeft(
    [](std::string _) { return std::string("b"); }
  );
  EXPECT_TRUE(rightMapped.isRight()) << "Expected right mapped value to be right, but it was not.";
}

TEST(Data_Either, MapBoth) {
  const auto leftE = Left(std::string("a")).to<int>();
  const auto rightE = Right(1).to<std::string>();

  const auto leftMapped = leftE.mapBoth(
    [](std::string _) { return 1; },
    [](int _) { return std::string("b");}
  );
  EXPECT_TRUE(leftMapped.isLeft()) << "Expected left mapped value to be left, but it was not.";
  leftMapped.left().ifSome([](const int& x) {
    EXPECT_EQ(x, 1) << "Expected left mapped value to be '1', but it was not.";
  });

  const auto rightMapped = rightE.mapBoth(
    [](std::string _) { return 1; },
    [](const int& x) { return std::to_string(x); }
  );
  EXPECT_TRUE(rightMapped.isRight()) << "Expected right mapped value to be right, but it was not.";
  rightMapped.right().ifSome([](const std::string& x) {
    EXPECT_EQ(x, "1") << "Expected right mapped value to be '1', but it was not.";
  });
}

TEST(Data_Either, FlatMap) {
  const auto leftE = Left(std::string("a")).to<int>();
  const auto rightE = Right(1).to<std::string>();

  const auto leftFlatMapped = leftE.flatMap(
    [](int _) { return LeftE<std::string>(std::string("b")); }
  );
  EXPECT_TRUE(leftFlatMapped.isLeft()) << "Expected left flat mapped value to be left, but it was not.";

  const auto rightFlatMapped = rightE.flatMap(
    [](const int& x) { return RightE<std::string>(std::to_string(x)); }
  );
  EXPECT_TRUE(rightFlatMapped.isRight()) << "Expected right flat mapped value to be right, but it was not.";
  rightFlatMapped.right().ifSome([](const std::string& x) {
    EXPECT_EQ(x, "1") << "Expected right flat mapped value to be '1', but it was not.";
  });
}

TEST(Data_Either, FlatMapLeft) {
  const auto leftE = Left(std::string("a")).to<int>();
  const auto rightE = Right(1).to<std::string>();

  const auto leftFlatMapped = leftE.flatMapLeft(
    [](std::string _) { return LeftE<int>(std::string("b")); }
  );
  EXPECT_TRUE(leftFlatMapped.isLeft()) << "Expected left flat mapped value to be left, but it was not.";
  leftFlatMapped.left().ifSome([](const std::string& str) {
    EXPECT_EQ(str, "b") << "Expected left flat mapped value to be 'b', but it was not.";
  });

  const auto rightFlatMapped = rightE.flatMapLeft(
    [](std::string _) { return RightE<int>(2); }
  );
  EXPECT_TRUE(rightFlatMapped.isRight()) << "Expected right flat mapped value to be right, but it was not.";
}

TEST(Data_Either, FlatMapBoth) {
  const auto leftE = Left(std::string("a")).to<int>();
  const auto rightE = Right(1).to<std::string>();

  const auto leftFlatMapped = leftE.flatMapBoth(
    [](const std::string& str) { return LeftE<int>(str); },
    [](int _) { return RightE<std::string>(1); }
  );
  EXPECT_TRUE(leftFlatMapped.isLeft()) << "Expected left flat mapped value to be left, but it was not.";
  leftFlatMapped.left().ifSome([](const std::string& str) {
    EXPECT_EQ(str, "a") << "Expected left flat mapped value to be 'a', but it was not.";
  });

  const auto rightFlatMapped = rightE.flatMapBoth(
    [](std::string _) { return LeftE<int>(std::string("b")); },
    [](const int& x) { return RightE<std::string>(x); }
  );
  EXPECT_TRUE(rightFlatMapped.isRight()) << "Expected right flat mapped value to be right, but it was not.";
  rightFlatMapped.right().ifSome([](const int& x) {
    EXPECT_EQ(x, 1) << "Expected right flat mapped value to be 1, but it was not.";
  });
}
