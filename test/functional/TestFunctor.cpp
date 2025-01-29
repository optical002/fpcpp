#include <core/data/Option.h>
#include <core/functional/Functor.h>
#include <gtest/gtest.h>

TEST(Functional_Functor, Functionality) {
  const auto opt = Some(1);
  const auto mapped = Functor::Map(opt, [](auto a) { return std::to_string(a); });

  EXPECT_TRUE(Equal(mapped, Some("1")));
}
