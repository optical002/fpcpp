#include "TestDebugStr.h"
#include <gtest/gtest.h>

TEST(Typeclasses_DebugStr, PartialSpecialization) {
  TestWithDebugStr a;
  ToDebugStr(a);
}

TEST(Typeclasses_DebugStr, Functionality) {
  int x1 = 1;
  EXPECT_EQ(ToDebugStr(x1), "Int(1)") << "Expected a to be equal to 1, but it was not.";

  unsigned int x2 = 2;
  EXPECT_EQ(ToDebugStr(x2), "UnsignedInt(2)") << "Expected a to be equal to 2, but it was not.";

  long x3 = 3;
  EXPECT_EQ(ToDebugStr(x3), "Long(3)") << "Expected a to be equal to 3, but it was not.";

  unsigned long x4 = 4;
  EXPECT_EQ(ToDebugStr(x4), "UnsignedLong(4)") << "Expected x4 to be equal to UnsignedLong(4), but it was not.";

  long long x5 = 5;
  EXPECT_EQ(ToDebugStr(x5), "LongLong(5)") << "Expected x5 to be equal to LongLong(5), but it was not.";

  unsigned long long x6 = 6;
  EXPECT_EQ(ToDebugStr(x6), "UnsignedLongLong(6)") << "Expected x6 to be equal to UnsignedLongLong(6), but it was not.";

  short x7 = 7;
  EXPECT_EQ(ToDebugStr(x7), "Short(7)") << "Expected x7 to be equal to Short(7), but it was not.";

  unsigned short x8 = 8;
  EXPECT_EQ(ToDebugStr(x8), "UnsignedShort(8)") << "Expected x8 to be equal to UnsignedShort(8), but it was not.";

  char x9 = 'a';
  EXPECT_EQ(ToDebugStr(x9), "Char(a)") << "Expected x9 to be equal to Char(a), but it was not.";

  bool x10 = true;
  EXPECT_EQ(ToDebugStr(x10), "Bool(true)") << "Expected x11 to be equal to Bool(true), but it was not.";

  float x11 = 12.34f;
  EXPECT_EQ(ToDebugStr(x11), "Float(12.34)") << "Expected x12 to be equal to Float(12.34), but it was not.";

  double x12 = 13.56;
  EXPECT_EQ(ToDebugStr(x12), "Double(13.56)") << "Expected x13 to be equal to Double(13.56), but it was not.";

  long double x13 = 14.78L;
  EXPECT_EQ(ToDebugStr(x13), "LongDouble(14.78)") << "Expected x14 to be equal to LongDouble(14.78), but it was not.";

  std::string x14 = "Hello";
  EXPECT_EQ(ToDebugStr(x14), "String(Hello)") << "Expected b to be equal to Hello, but it was not.";
}