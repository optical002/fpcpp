#include <core/reactive/RxRef.h>
#include <core/reactive/Tracker.h>
#include <core/data/Enums.h>
#include <gtest/gtest.h>

TEST(Reactive_RxRef, Functionality) {
  auto rxRef = RxRef<int>::a(1);
  auto disposableTracker = DisposableTracker();
  EXPECT_EQ(1, rxRef.getValue());

  int refValue = 0;
  rxRef.subscribe(disposableTracker, [&refValue](const int& newValue) {
    refValue = newValue;
  });

  rxRef.setValue(2);
  EXPECT_EQ(2, rxRef.getValue());
  EXPECT_EQ(2, refValue);

  RxVal<int> rxVal = rxRef;
  EXPECT_EQ(2, rxVal.getValue());

  int valValue = 0;
  rxVal.subscribe(disposableTracker, [&valValue](const int& newValue) {
    valValue = newValue;
  });
  EXPECT_EQ(2, valValue);

  rxRef.setValue(3);
  EXPECT_EQ(3, rxRef.getValue());
  EXPECT_EQ(3, refValue);
  EXPECT_EQ(3, rxVal.getValue());
  EXPECT_EQ(3, valValue);

  int refValue2 = 0;
  rxRef.subscribe(disposableTracker, [&refValue2](const int& newValue) {
    refValue2 = newValue;
  }, SubscribeBehaviour::InvokeOnNextVal);
  EXPECT_EQ(0, refValue2);

  int valValue2 = 0;
  rxVal.subscribe(disposableTracker, [&valValue2](const int& newValue) {
    valValue2 = newValue;
  }, SubscribeBehaviour::InvokeOnNextVal);
  EXPECT_EQ(0, valValue2);

  rxRef.setValue(4);
  EXPECT_EQ(4, rxRef.getValue());
  EXPECT_EQ(4, refValue);
  EXPECT_EQ(4, refValue2);
  EXPECT_EQ(4, rxVal.getValue());
  EXPECT_EQ(4, valValue);
  EXPECT_EQ(4, valValue2);
}

TEST(Reactive_RxRef, Map) {
  auto rxRef = RxRef<int>::a(1);
  auto disposableTracker = DisposableTracker();

  auto rxVal = rxRef.map([](const int& value) {
    return value * 2;
  });

  EXPECT_EQ(1, rxRef.getValue());
  EXPECT_EQ(2, rxVal.getValue());

  int refValue = 0;
  int valValue = 0;
  rxRef.subscribe(disposableTracker, [&refValue](const int& newValue) {
    refValue = newValue * 2;
  });
  EXPECT_EQ(1, rxRef.getValue());
  EXPECT_EQ(2, rxVal.getValue());
  EXPECT_EQ(2, refValue);

  rxVal.subscribe(disposableTracker, [&valValue](const int& newValue) {
    valValue = newValue * 3;
  });
  EXPECT_EQ(1, rxRef.getValue());
  EXPECT_EQ(2, rxVal.getValue());
  EXPECT_EQ(2, refValue);
  EXPECT_EQ(6, valValue);

  rxRef.setValue(2);
  EXPECT_EQ(2, rxRef.getValue());
  EXPECT_EQ(4, rxVal.getValue());
  EXPECT_EQ(4, refValue);
  EXPECT_EQ(12, valValue);
}

TEST(Reactive_RxRef, FlatMap) {
  auto rxRef = RxRef<int>::a(1);
  auto disposableTracker = DisposableTracker();

  auto rxVal1 = rxRef.flatMap([](const int& value) {
    return RxRef<int>::a(value * 2);
  });

  auto rxVal2 = rxRef.flatMap([](const int& value) {
    return RxVal<int>::a(value * 3);
  });

  EXPECT_EQ(2, rxVal1.getValue());
  EXPECT_EQ(3, rxVal2.getValue());

  rxRef.setValue(2);

  EXPECT_EQ(4, rxVal1.getValue());
  EXPECT_EQ(6, rxVal2.getValue());
}