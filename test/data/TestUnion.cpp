#include "TestUnion.h"
#include <gtest/gtest.h>

TEST(Data_Union, Kind_Eq) {
  StateKind kind1 = StateKind::age, kind2 = StateKind::age, kind3 = StateKind::cost;
  EXPECT_TRUE(Equal(kind1, kind2)) << "Expected 'kind1' and 'kind2' to be equal.";
  EXPECT_FALSE(Equal(kind1, kind3)) << "Expected 'kind1' and 'kind3' to not be equal.";

  StateAKind aKind1 = StateAKind::raw, aKind2 = StateAKind::raw, aKind3 = StateAKind::something;
  EXPECT_TRUE(Equal(aKind1, aKind2)) << "Expected 'aKind1' and 'aKind2' to be equal.";
  EXPECT_FALSE(Equal(aKind1, aKind3)) << "Expected 'aKind1' and 'aKind3' to not be equal.";
}

TEST(Data_Union, Kind_ToString) {
  StateKind kind1 = StateKind::age, kind2 = StateKind::name;
  EXPECT_EQ(ToStr(kind1), "age");
  EXPECT_EQ(ToStr(kind2), "name");

  StateAKind aKind1 = StateAKind::raw, aKind2 = StateAKind::something;
  EXPECT_EQ(ToStr(aKind1), "raw");
  EXPECT_EQ(ToStr(aKind2), "something");
}

TEST(Data_Union, StaticConstructors) {
  State stateAge = State::create_age(1), stateName = State::create_name("Hello");
  EXPECT_EQ(stateAge.kind(), StateKind::age);
  EXPECT_EQ(stateName.kind(), StateKind::name);

  StateA<int> stateRaw = StateA<int>::create_raw(69);
  EXPECT_EQ(stateRaw.kind(), StateAKind::raw);
}

TEST(Data_Union, Getters) {
  State stateAge = State::create_age(1);
  auto maybeAge = stateAge.as_age();
  EXPECT_TRUE(maybeAge.isSome());
  EXPECT_EQ(maybeAge.getOr(1), 1);

  StateA<int> stateRaw = StateA<int>::create_raw(69);
  auto maybeRaw = stateRaw.as_raw();
  EXPECT_TRUE(maybeRaw.isSome());
  EXPECT_EQ(maybeRaw.getOr(1), 69);
}

TEST(Data_Union, Fold) {
  State stateAge = State::create_age(1);
  auto value = stateAge.fold(
    [](auto _) { return 0; },
    [](int age) { return age; },
    [](auto _) { return 2; },
    [](auto _) { return 2; }
  );
  EXPECT_EQ(value, 1);

  StateA<int> stateRaw = StateA<int>::create_raw(69);
  value = stateRaw.fold(
    [](int raw) { return raw; },
    [](auto _) { return 2; },
    [](auto _) { return 2; },
    [](auto _) { return 2; }
  );
  EXPECT_EQ(value, 69);
}

TEST(Data_Union, VoidFold) {
  State stateAge = State::create_age(1);
  int value = -1;
  stateAge.voidFold(
    [&value](auto _) { value = 0; },
    [&value](int age) { value = age; },
    [&value](auto _) { value = 2; },
    [&value](auto _) { value = 2; }
  );
  EXPECT_EQ(value, 1);

  StateA<int> stateRaw = StateA<int>::create_raw(69);
  stateRaw.fold(
    [&value](int raw) { value = raw; },
    [&value](auto _) { value = 0; },
    [&value](auto _) { value = 0; },
    [&value](auto _) { value = 0; }
  );
  EXPECT_EQ(value, 69);
}

TEST(Data_Union, Eq) {
  State
    stateAge1 = State::create_age(1),
    stateAge2 = State::create_age(1),
    stateAge3 = State::create_age(2),
    stateName = State::create_name("Hello");

  EXPECT_TRUE(Equal(stateAge1, stateAge2)) << "Expected 'stateAge1' and 'stateAge2' to be equal.";
  EXPECT_FALSE(Equal(stateAge1, stateAge3)) << "Expected 'stateAge1' and 'stateAge3' to not be equal.";
  EXPECT_FALSE(Equal(stateAge1, stateName)) << "Expected 'stateAge1' and 'stateName' to not be equal.";

  StateA<int>
    stateRaw1 = StateA<int>::create_raw(69),
    stateRaw2 = StateA<int>::create_raw(69),
    stateRaw3 = StateA<int>::create_raw(420),
    stateSomething = StateA<int>::create_something({});

  EXPECT_TRUE(Equal(stateRaw1, stateRaw2)) << "Expected 'stateRaw1' and 'stateRaw2' to be equal.";
  EXPECT_FALSE(Equal(stateRaw1, stateRaw3)) << "Expected 'stateRaw1' and 'stateRaw3' to not be equal.";
  EXPECT_FALSE(Equal(stateRaw1, stateSomething)) << "Expected 'stateRaw1' and 'stateSomething' to not be equal.";
}

TEST(Data_Union, ToString) {
  State stateAge = State::create_age(1);
  EXPECT_EQ(ToStr(stateAge), "State(kind=age, data=Int(1))");

  StateA<int> stateRaw = StateA<int>::create_raw(69);
  EXPECT_EQ(ToStr(stateRaw), "StateA(kind=raw, data=Int(69))");
}