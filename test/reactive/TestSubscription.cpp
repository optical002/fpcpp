#include <core/reactive/Subscription.h>
#include <gtest/gtest.h>
#include <functional>

TEST(Reactive_Subscription, Functionality) {
  int value = 0;
  auto subscription = Subscription([&value] { value = 1; });
  EXPECT_EQ(value, 0) << "After creating a subscription, it should not unsubscribe.";

  const auto hasUnsubscribed = subscription.unsubscribe();
  EXPECT_EQ(value, 1) << "Unsubscribing should invoke lambda.";
  EXPECT_TRUE(hasUnsubscribed) << "Unsubscribing for the first time should return true.";

  value = 0;
  const auto hasUnsubscribed2 = subscription.unsubscribe();
  EXPECT_EQ(value, 0) << "Unsubscribing for the second time should not invoke lambda.";
  EXPECT_FALSE(hasUnsubscribed2) << "Unsubscribing for the second time should return false.";
}
