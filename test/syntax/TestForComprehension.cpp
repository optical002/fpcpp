#include <core/data/Future.h>
#include <core/data/Option.h>
#include <core/syntax/Try.h>
#include <core/syntax/ForComprehension.h>
#include <gtest/gtest.h>

TEST(Syntax_ForComprehension, Map) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](const int& x) { return x + 1; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 2) << "Expected to be '2', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](const int& x) { return x + 1; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 2) << "Expected to be '2', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap1SingleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](const int& x) { return Some(x + 1); },
    [](const int& x) { return x;}
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 2) << "Expected to be '2', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](const int& x) { return Successful(x + 1); },
    [](const int& x) { return x;}
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 2) << "Expected to be '2', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap1MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](const int& x) { return Some(x + 1); },
    [](const int& a, const int& b) { return a + b;}
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 3) << "Expected to be '3', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](const int& x) { return Successful(x + 1); },
    [](const int& a, const int& b) { return a + b;}
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 3) << "Expected to be '3', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap2MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return x1 + x2 + x3; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 6) << "Expected to be '6', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return x1 + x2 + x3; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 6) << "Expected to be '6', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap3MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return Some(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return x1 + x2 + x3 + x4; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 12) << "Expected to be '12', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return Successful(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return x1 + x2 + x3 + x4; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 12) << "Expected to be '12', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap4MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return Some(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Some(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return x1 + x2 + x3 + x4 + x5; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 24) << "Expected to be '24', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return Successful(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Successful(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return x1 + x2 + x3 + x4 + x5; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 24) << "Expected to be '24', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap5MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return Some(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Some(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Some(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return x1 + x2 + x3 + x4 + x5 + x6; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 48) << "Expected to be '48', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return Successful(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Successful(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Successful(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return x1 + x2 + x3 + x4 + x5 + x6; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 48) << "Expected to be '48', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap6MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return Some(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Some(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Some(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Some(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return x1 + x2 + x3 + x4 + x5 + x6 + x7; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 96) << "Expected to be '96', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return Successful(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Successful(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Successful(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Successful(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return x1 + x2 + x3 + x4 + x5 + x6 + x7; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 96) << "Expected to be '96', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap7MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return Some(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Some(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Some(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Some(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return Some(x1 + x2 + x3 + x4 + x5 + x6 + x7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 192) << "Expected to be '192', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return Successful(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Successful(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Successful(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Successful(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return Successful(x1 + x2 + x3 + x4 + x5 + x6 + x7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 192) << "Expected to be '192', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap8MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return Some(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Some(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Some(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Some(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return Some(x1 + x2 + x3 + x4 + x5 + x6 + x7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return Some(x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 384) << "Expected to be '384', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return Successful(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Successful(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Successful(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Successful(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return Successful(x1 + x2 + x3 + x4 + x5 + x6 + x7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return Successful(x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 384) << "Expected to be '384', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap9MultipleArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    [](int x1) { return Some(x1 + 1); },
    [](int x1, int x2) { return Some(x1 + x2); },
    [](int x1, int x2, int x3) { return Some(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Some(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Some(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Some(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return Some(x1 + x2 + x3 + x4 + x5 + x6 + x7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return Some(x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9) { return Some(x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9, int x10) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 768) << "Expected to be '768', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    [](int x1) { return Successful(x1 + 1); },
    [](int x1, int x2) { return Successful(x1 + x2); },
    [](int x1, int x2, int x3) { return Successful(x1 + x2 + x3); },
    [](int x1, int x2, int x3, int x4) { return Successful(x1 + x2 + x3 + x4); },
    [](int x1, int x2, int x3, int x4, int x5) { return Successful(x1 + x2 + x3 + x4 + x5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return Successful(x1 + x2 + x3 + x4 + x5 + x6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return Successful(x1 + x2 + x3 + x4 + x5 + x6 + x7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return Successful(x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9) { return Successful(x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9, int x10) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 768) << "Expected to be '768', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap1MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    [](const int& a, const int& b) { return a + b;}
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 2) << "Expected to be '2', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    [](const int& a, const int& b) { return a + b;}
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 2) << "Expected to be '2', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap2MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    [](int x1, int x2, int x3) { return x1 + x2 + x3; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 4) << "Expected to be '4', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    [](int x1, int x2, int x3) { return x1 + x2 + x3; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 4) << "Expected to be '4', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap3MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    []{ return Some(3); },
    [](int x1, int x2, int x3, int x4) { return x1 + x2 + x3 + x4; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 7) << "Expected to be '7', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    []{ return Successful(3); },
    [](int x1, int x2, int x3, int x4) { return x1 + x2 + x3 + x4; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 7) << "Expected to be '7', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap4MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    []{ return Some(3); },
    []{ return Some(4); },
    [](int x1, int x2, int x3, int x4, int x5) { return x1 + x2 + x3 + x4 + x5; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 11) << "Expected to be '11', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    []{ return Successful(3); },
    []{ return Successful(4); },
    [](int x1, int x2, int x3, int x4, int x5) { return x1 + x2 + x3 + x4 + x5; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 11) << "Expected to be '11', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap5MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    []{ return Some(3); },
    []{ return Some(4); },
    []{ return Some(5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return x1 + x2 + x3 + x4 + x5 + x6; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 16) << "Expected to be '16', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    []{ return Successful(3); },
    []{ return Successful(4); },
    []{ return Successful(5); },
    [](int x1, int x2, int x3, int x4, int x5, int x6) { return x1 + x2 + x3 + x4 + x5 + x6; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 16) << "Expected to be '16', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap6MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    []{ return Some(3); },
    []{ return Some(4); },
    []{ return Some(5); },
    []{ return Some(6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return x1 + x2 + x3 + x4 + x5 + x6 + x7; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 22) << "Expected to be '22', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    []{ return Successful(3); },
    []{ return Successful(4); },
    []{ return Successful(5); },
    []{ return Successful(6); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7) { return x1 + x2 + x3 + x4 + x5 + x6 + x7; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 22) << "Expected to be '22', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap7MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    []{ return Some(3); },
    []{ return Some(4); },
    []{ return Some(5); },
    []{ return Some(6); },
    []{ return Some(7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 29) << "Expected to be '29', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    []{ return Successful(3); },
    []{ return Successful(4); },
    []{ return Successful(5); },
    []{ return Successful(6); },
    []{ return Successful(7); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 29) << "Expected to be '29', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap8MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    []{ return Some(3); },
    []{ return Some(4); },
    []{ return Some(5); },
    []{ return Some(6); },
    []{ return Some(7); },
    []{ return Some(8); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 37) << "Expected to be '37', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    []{ return Successful(3); },
    []{ return Successful(4); },
    []{ return Successful(5); },
    []{ return Successful(6); },
    []{ return Successful(7); },
    []{ return Successful(8); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 37) << "Expected to be '37', but it was not.";
  });
}

TEST(Syntax_ForComprehension, FlatMap9MapArg) {
  const auto some = Some(1);
  const auto comprehended = ForComprehension(some,
    []{ return Some(1); },
    []{ return Some(2); },
    []{ return Some(3); },
    []{ return Some(4); },
    []{ return Some(5); },
    []{ return Some(6); },
    []{ return Some(7); },
    []{ return Some(8); },
    []{ return Some(9); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9, int x10) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10; }
  );
  EXPECT_TRUE(comprehended.isSome()) << "Expected comprehended to be 'Some', but it was not.";
  comprehended.ifSome([](int x) {
    EXPECT_EQ(x, 46) << "Expected to be '46', but it was not.";
  });

  auto promise = Promise<int>();
  auto future = promise.getFuture();
  auto comprehendedFuture = ForComprehension(future,
    []{ return Successful(1); },
    []{ return Successful(2); },
    []{ return Successful(3); },
    []{ return Successful(4); },
    []{ return Successful(5); },
    []{ return Successful(6); },
    []{ return Successful(7); },
    []{ return Successful(8); },
    []{ return Successful(9); },
    [](int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9, int x10) { return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10; }
  );
  promise.tryComplete(1);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Expected comprehendedFuture to be completed, but it was not.";
  comprehendedFuture.onComplete([](int x) {
    EXPECT_EQ(x, 46) << "Expected to be '46', but it was not.";
  });
}
