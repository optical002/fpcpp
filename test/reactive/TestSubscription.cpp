#include "core/reactive/Subscription.h"
#include "gtest/gtest.h"
#include <functional>

namespace reactive {
  TEST(Subscription, Functionality) {
    int value = 0;
    auto subscription = Subscription([&value] {
      value = 1;
    });
    EXPECT_EQ(value, 0) << "After creating a subscription, it should not unsubscribe.";

    const auto hasUnsubscribed = subscription.unsubscribe();
    EXPECT_EQ(value, 1) << "Unsubscribing should invoke lambda.";
    EXPECT_TRUE(hasUnsubscribed) << "Unsubscribing for the first time should return true.";

    value = 0;
    const auto hasUnsubscribed2 = subscription.unsubscribe();
    EXPECT_EQ(value, 0) << "Unsubscribing for the second time should not invoke lambda.";
    EXPECT_FALSE(hasUnsubscribed2) << "Unsubscribing for the second time should return false.";

    const auto subscription2 = Subscription::create([&value] {
      value = 1;
    });
    EXPECT_EQ(value, 0) << "After creating a subscription2 ptr, it should not unsubscribe.";
    subscription2->unsubscribe();
    EXPECT_EQ(value, 1) << "Unsubscribing from subscription ptr should invoke lambda.";
  }
  TEST(Tracker, test) {
    EXPECT_EQ(1, 1);
  }
}
