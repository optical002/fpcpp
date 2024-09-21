#include <core/reactive/Tracker.h>
#include <gtest/gtest.h>

TEST(Reactive_Tracker, DisposableTracker) {
  auto disposableTracker = DisposableTracker();
  Tracker* tracker = &disposableTracker;

  int value = 0;
  tracker->track([&value] { value = 1;});
  EXPECT_EQ(value, 0) << "After tracking an action, it should not be invoked.";

  disposableTracker.dispose();
  EXPECT_EQ(value, 1) << "After disposing the disposable tracker, the trackers tracked action should be invoked.";

  auto disposableTracker2 = DisposableTracker();
  auto subscription = Subscription::create([&value] { value = 2; });
  disposableTracker2.track(subscription);
  EXPECT_EQ(value, 1) << "After tracking a subscription, it should not be unsubscribed.";

  disposableTracker2.dispose();
  EXPECT_EQ(value, 2) << "After disposing the disposable tracker, the trackers tracked subscription should be unsubscribed.";
}

TEST(Reactive_Tracker, NoOpTracker) {
  int value = 0;
  NoOpTracker{}.track([&value] { value = 1; });
  EXPECT_EQ(value, 0) << "After tracking an action, it should not be invoked.";
}
