#include "TestUnion.h"
#include <gtest/gtest.h>

TEST(Data_Union, Kind_Eq) {
  StateKind kind1 = StateKind::age, kind2 = StateKind::age, kind3 = StateKind::cost;

  EXPECT_TRUE(Equal(kind1, kind2)) << "Expected 'kind1' and 'kind2' to be equal.";
  EXPECT_FALSE(Equal(kind1, kind3)) << "Expected 'kind1' and 'kind3' to not be equal.";
}

TEST(Data_Union, Kind_ToString) {
  StateKind kind1 = StateKind::age, kind2 = StateKind::name;

  EXPECT_EQ(ToStr(kind1), "age");
  EXPECT_EQ(ToStr(kind2), "name");
}

TEST(Data_Union, StaticConstructors) {
  State stateAge = State::create_age(1), stateName = State::create_name("Hello");

  EXPECT_EQ(stateAge.kind, StateKind::age);
  EXPECT_EQ(stateName.kind, StateKind::name);
}

TEST(Data_Union, Getters) {
  State stateAge = State::create_age(1);
  auto maybeAge = stateAge.as_age();
  EXPECT_TRUE(maybeAge.isSome());
  EXPECT_EQ(maybeAge.getOr(1), 1);
}

TEST(Data_Union, Fold) {
  State stateAge = State::create_age(1);
  auto value = stateAge.fold(
    [](auto _) { return 0; },
    [](int age) { return age; },
    [](auto _) { return 2; }
  );
  EXPECT_EQ(value, 1);
}

TEST(Data_Union, VoidFold) {
  State stateAge = State::create_age(1);
  int value = -1;
  stateAge.voidFold(
    [&value](auto _) { value = 0; },
    [&value](int age) { value = age; },
    [&value](auto _) { value = 2; }
  );
  EXPECT_EQ(value, 1);
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
}

TEST(Data_Union, ToString) {
  State stateAge = State::create_age(1);

  EXPECT_EQ(ToStr(stateAge), "State(kind=age, data=Int(1))");
}