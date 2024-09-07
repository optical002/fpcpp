#include "TestRecord.h"
#include <gtest/gtest.h>

namespace data {
  TEST(Record, Equality) {
    // TestNoParams x1;
    std::string gen = "a";
    EXPECT_EQ(gen, "") << "Expected to be equal, but it was not.";
  }
}
