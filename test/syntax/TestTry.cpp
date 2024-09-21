#include <core/syntax/Try.h>
#include <core/syntax/ForComprehension.h>
#include <gtest/gtest.h>

TEST(Syntax_Try, Try) {
  const auto trySuccessful = Try([]{
    return 1;
  });
  EXPECT_TRUE(trySuccessful.isRight()) << "Expected trySuccessful to be 'Success', but it was not.";

  const auto tryError = Try([] {
    throw std::runtime_error("Error");
  });
  EXPECT_TRUE(tryError.isLeft()) << "Expected tryError to be 'Failure', but it was not.";
}

TEST(Syntax_Try, TryLogSet) {
  int value = 0;
  int errorLog = 0;
  TryLogSet(
    [] { return 1; }, value, [&errorLog](std::string _) {
      errorLog = 1;
    }
  );
  EXPECT_EQ(value, 1) << "Expected value to be 1, but it was not.";
  EXPECT_EQ(errorLog, 0) << "Expected errorLog to be 0, but it was not.";

  TryLogSet(
    [] { throw std::runtime_error("Error"); }, value, [&errorLog](std::string _) {
      errorLog = 1;
    }
  );
  EXPECT_EQ(value, 1) << "Expected value to be 1, but it was not.";
  EXPECT_EQ(errorLog, 1) << "Expected errorLog to be 1, but it was not.";
}

struct A { };
struct B { A* a; };
struct C { B* b; };

TEST(Syntax_Try, TryPtr) {
  A* nullPtr = nullptr;
  const auto tryNull = TryPtr(nullPtr);
  EXPECT_TRUE(tryNull.isLeft()) << "Expected tryNull to be 'Failure', but it was not.";

  A notNull;
  A* notNullPtr = &notNull;
  const auto tryNotNull = TryPtr(notNullPtr);
  EXPECT_TRUE(tryNotNull.isRight()) << "Expected tryNotNull to be 'Success', but it was not.";

  A a;
  B b = { &a };
  C c = { &b };
  C* cPtr = &c;
  const auto tryNotNullPtrChain = ForComprehension(TryPtr(cPtr),
    [](C* c) { return TryPtr(c->b); },
    [](B* b) { return TryPtr(b->a); },
    [](SafePtr<A*> a) { return a; }
  );
  EXPECT_TRUE(tryNotNullPtrChain.isRight()) << "Expected tryNotNullPtrChain to be 'Success', but it was not.";

  B* nullBPtr = nullptr;
  C cNull = { nullBPtr };
  C* cNullPtr = &cNull;
  const auto tryNullPtrChain = ForComprehension(TryPtr(cNullPtr),
    [](C* c) { return TryPtr(c->b); },
    [](B* b) { return TryPtr(b->a); },
    [](SafePtr<A*> a) { return a; }
  );
  EXPECT_TRUE(tryNullPtrChain.isLeft()) << "Expected tryNullPtrChain to be 'Failure', but it was not.";
}
