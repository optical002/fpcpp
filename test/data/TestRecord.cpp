#include "TestRecord.h"
#include <gtest/gtest.h>

namespace data {
  TEST(Record, Equality) {
    TestNoParams x1, x2;
    EXPECT_TRUE(x1 == x2) << "Expected to be equal, but it was not.";
  }
}
