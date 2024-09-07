#include "core/reactive/Observable.h"
#include "core/reactive/Tracker.h"
#include "gtest/gtest.h"

namespace reactive {
  TEST(Observable, Functionality) {
    auto subject = Subject<int>();
    auto disposableTracker = DisposableTracker();
    Tracker* tracker = &disposableTracker;

    int value = 0;
    subject.subscribe(tracker, [&value](const int& newValue) {
      value = newValue;
    });
    EXPECT_EQ(value, 0) << "After subscribing to a subject, the value should not be changed.";

    subject.push(1);
    EXPECT_EQ(value, 1) << "After pushing a value 1 to the subject, the value should be changed.";

    subject.push(2);
    EXPECT_EQ(value, 2) << "After pushing a value 2 to the subject second time, the value should be changed.";

    disposableTracker.dispose();
    subject.push(3);
    EXPECT_EQ(value, 2) << "After disposing the disposable tracker, the subscription should be unsubscribed.";

    auto disposableTracker2 = DisposableTracker();
    Tracker* tracker2 = &disposableTracker2;

    int value2 = 0;
    subject.subscribe(tracker, [&value2](const int& newValue) {
      value2 = newValue;
    });

    int value3 = 0;
    subject.subscribe(tracker2, [&value3](const int& newValue) {
      value3 = newValue;
    });

    int value4 = 0;
    subject.subscribe(tracker, [&value4](const int& newValue) {
      value4 = newValue;
    });

    subject.push(4);
    EXPECT_EQ(value2, 4) << "After pushing a value 4 to the subject, the value2 should be changed.";
    EXPECT_EQ(value3, 4) << "After pushing a value 4 to the subject, the value3 should be changed.";
    EXPECT_EQ(value4, 4) << "After pushing a value 4 to the subject, the value4 should be changed.";

    disposableTracker2.dispose();
    subject.push(5);
    
    EXPECT_EQ(value2, 5) << "After pushing a value 5 to the subject, the value2 should be changed.";
    EXPECT_EQ(value3, 4) << "After pushing a value 5 to the subject, the value3 should not be changed.";
    EXPECT_EQ(value4, 5) << "After pushing a value 5 to the subject, the value4 should be changed.";
  }

  TEST(Observable, Map) {
    auto subject = Subject<int>();
    auto disposableTracker = DisposableTracker();
    Tracker* tracker = &disposableTracker;

    const auto mappedObservable = subject.map([](const int& value) {
      return std::to_string(value);
    });

    std::string value;
    mappedObservable->subscribe(tracker, [&value](const std::string& newValue) {
      value = newValue;
    });

    subject.push(1);
    EXPECT_EQ(value, "1") << "After pushing a value 1 to the subject, the value should be changed to '1'.";
  }

  TEST(Observable, Filter) {
    auto subject = Subject<int>();
    auto disposableTracker = DisposableTracker();
    Tracker* tracker = &disposableTracker;

    int value = 0;
    const auto filteredObservable = subject.filter([](const int& value) {
      return value % 2 == 0;
    });
    filteredObservable->subscribe(tracker, [&value](const int& newValue) {
      value = newValue;
    });
    EXPECT_EQ(value, 0) << "After subscribing to a filtered subject, the value should not be changed.";

    subject.push(1);
    EXPECT_EQ(value, 0) << "After pushing a value 1 to the subject, the value should not be changed.";

    subject.push(2);
    EXPECT_EQ(value, 2) << "After pushing a value 2 to the subject, the value should be changed to 2.";

    subject.push(3);
    EXPECT_EQ(value, 2) << "After pushing a value 3 to the subject, the value should not be changed.";

    subject.push(4);
    EXPECT_EQ(value, 4) << "After pushing a value 4 to the subject, the value should be changed to 4.";
  }

  TEST(Observable, Join) {
    auto subject1 = Subject<int>();
    auto subject2 = Subject<int>();
    auto subject3 = Subject<int>();
    auto disposableTracker = DisposableTracker();
    Tracker* tracker = &disposableTracker;

    const auto joinedObservable = subject1.join(&subject2, &subject3);
    int value = 0;
    joinedObservable->subscribe(tracker, [&value](const int& newValue) {
      value = newValue;
    });

    subject1.push(1);
    EXPECT_EQ(value, 1) << "After pushing a value 1 to the subject1, the value should be changed to 1.";

    subject2.push(2);
    EXPECT_EQ(value, 2) << "After pushing a value 2 to the subject2, the value should be changed to 2.";

    subject3.push(3);
    EXPECT_EQ(value, 3) << "After pushing a value 3 to the subject3, the value should be changed to 3.";
  }

  TEST(Observable, ToFuture) {
    auto subject = Subject<int>();
    auto future = subject.toFuture();
    
    int value = 0;
    future->onComplete([&value](const int& newValue) {
      value = newValue;
    });
    EXPECT_EQ(value, 0) << "After creating a future from a subject, the value should not be changed.";

    subject.push(1);
    EXPECT_EQ(value, 1) << "After pushing a value 1 to the subject, the value should be changed to 1.";

    subject.push(2);
    EXPECT_EQ(value, 1) << "After pushing a value 2 to the subject, the value should not change to 2.";
  }

  struct A {
    int a;
  };

  TEST(Observable, Changes) {
    auto subject = Subject<int>();
    auto disposableTracker = DisposableTracker();
    Tracker* tracker = &disposableTracker;

    int valuePrevious = 0;
    int valueNext = 0;
    const auto changesObservable = subject.changes();
    changesObservable->subscribe(tracker,
      [&valuePrevious, &valueNext](const Changes<int>& changes) {
        valuePrevious = changes.previous();
        valueNext = changes.next();
      }
    );

    EXPECT_EQ(valuePrevious, 0) << "After subscribing to a changes subject, the previous value should be 0.";
    EXPECT_EQ(valueNext, 0) << "After subscribing to a changes subject, the next value should be 0.";

    subject.push(1);

    EXPECT_EQ(valuePrevious, 0) << "After pushing first time to a changes subject, the previous value should be 0.";
    EXPECT_EQ(valueNext, 0) << "After pushing first time to a changes subject, the next value should be 0.";

    subject.push(2);

    EXPECT_EQ(valuePrevious, 1) << "After pushing second time to a changes subject, the previous value should be 1.";
    EXPECT_EQ(valueNext, 2) << "After pushing second time to a changes subject, the next value should be 2.";

    subject.push(3);

    EXPECT_EQ(valuePrevious, 2) << "After pushing third time to a changes subject, the previous value should be 2.";
    EXPECT_EQ(valueNext, 3) << "After pushing third time to a changes subject, the next value should be 3.";
  }
}
