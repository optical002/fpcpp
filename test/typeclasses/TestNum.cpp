#include "TestNum.h"
#include <gtest/gtest.h>

namespace typeclasses {
  TEST(Num, PartialSpecialization) {
    TestWithNum a, b;
    Num<TestWithNum>::add(a, b);
    Num<TestWithNum>::subtract(a, b);
    Num<TestWithNum>::multiply(a, b);
    Num<TestWithNum>::divide(a, b);
    Num<TestWithNum>::mod(a, b);
  }

  TEST(Num, Functionality) {
    int a = 2, b = 1;
    EXPECT_EQ(Num<int>::add(a, b), 3);
    EXPECT_EQ(Num<int>::subtract(a, b), 1);
    EXPECT_EQ(Num<int>::multiply(a, b), 2);
    EXPECT_EQ(Num<int>::divide(a, b), 2);
    EXPECT_EQ(Num<int>::mod(a, b), 0);
  }
}
