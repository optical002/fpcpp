#include <core/functional/RefCmp.h>
#include <gtest/gtest.h>

struct A { int a; };

TEST(Functional_RefCmp, Functionality) {
  const auto a = A{ 1 }, b = A{ 2 };

  EXPECT_FALSE(refCmp(a, b));
  EXPECT_TRUE(refCmp(a, a));
}
