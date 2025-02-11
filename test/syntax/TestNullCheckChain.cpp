#include <core/syntax/NullCheckChain.h>
#include <gtest/gtest.h>

struct D { };
struct C {
  D* d;
};
struct B {
  C* c;
};
struct A {
  B* b;
};

TEST(Syntax_NullCheckChain, Get1) {
  A* a1 = nullptr;
  auto result1 = NullCheckChain(a1, [](auto _) { return _->b;});
  EXPECT_EQ(result1, nullptr);

  auto a2 = A{ nullptr };
  auto a2Ptr = &a2;
  auto result2 = NullCheckChain(a2Ptr, [](auto _) { return _->b;} );
  EXPECT_EQ(result2, nullptr);

  auto b3 = B{ nullptr };
  auto a3 = A{ &b3 };
  auto a3Ptr = &a3;
  auto result3 = NullCheckChain(a3Ptr, [](auto _) { return _->b; });
  EXPECT_TRUE(result3 != nullptr);
}

TEST(Syntax_NullCheckChain, Get2) {
  A* a1 = nullptr;
  auto result1 = NullCheckChain(a1,
    [](auto _) { return _->b;},
    [](auto _) { return _->c;}
  );
  EXPECT_EQ(result1, nullptr);

  auto a2 = A{ nullptr };
  auto a2Ptr = &a2;
  auto result2 = NullCheckChain(a2Ptr,
    [](auto _) { return _->b;},
    [](auto _) { return _->c;}
  );
  EXPECT_EQ(result2, nullptr);

  auto b3 = B{ nullptr };
  auto a3 = A{ &b3 };
  auto a3Ptr = &a3;
  auto result3 = NullCheckChain(a3Ptr,
    [](auto _) { return _->b; },
    [](auto _) { return _->c; }
  );
  EXPECT_EQ(result3, nullptr);

  auto c4 = C{ nullptr };
  auto b4 = B{ &c4 };
  auto a4 = A{ &b4 };
  auto a4Ptr = &a4;
  auto result4 = NullCheckChain(a4Ptr,
    [](auto _) { return _->b; },
    [](auto _) { return _->c; }
  );
  EXPECT_TRUE(result4 != nullptr);
}


TEST(Syntax_NullCheckChain, Get3) {
  A* a1 = nullptr;
  auto result1 = NullCheckChain(a1,
    [](auto _) { return _->b;},
    [](auto _) { return _->c;},
    [](auto _) { return _->d;}
  );
  EXPECT_EQ(result1, nullptr);

  auto a2 = A{ nullptr };
  auto a2Ptr = &a2;
  auto result2 = NullCheckChain(a2Ptr,
    [](auto _) { return _->b;},
    [](auto _) { return _->c;},
    [](auto _) { return _->d;}
  );
  EXPECT_EQ(result2, nullptr);

  auto b3 = B{ nullptr };
  auto a3 = A{ &b3 };
  auto a3Ptr = &a3;
  auto result3 = NullCheckChain(a3Ptr,
    [](auto _) { return _->b; },
    [](auto _) { return _->c; },
    [](auto _) { return _->d;}
  );
  EXPECT_EQ(result3, nullptr);

  auto c4 = C{ nullptr };
  auto b4 = B{ &c4 };
  auto a4 = A{ &b4 };
  auto a4Ptr = &a4;
  auto result4 = NullCheckChain(a4Ptr,
    [](auto _) { return _->b; },
    [](auto _) { return _->c; },
    [](auto _) { return _->d;}
  );
  EXPECT_EQ(result4, nullptr);

  auto d5 = D{};
  auto c5 = C{ &d5 };
  auto b5 = B{ &c5 };
  auto a5 = A{ &b5 };
  auto a5Ptr = &a5;
  auto result5 = NullCheckChain(a5Ptr,
    [](auto _) { return _->b; },
    [](auto _) { return _->c; },
    [](auto _) { return _->d;}
  );
  EXPECT_TRUE(result5 != nullptr); // NullCheckGroup
}
