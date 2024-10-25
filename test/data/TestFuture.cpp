#include <core/data/Future.h>
#include <core/syntax/ForComprehension.h>
#include <core/typeclasses/Str.h>
#include <gtest/gtest.h>

void testCompilationFuture() {
  const auto future = Successful("a");
}

TEST(Data_Future, ForComprehension) {
  const auto promise1 = Promise<int>();
  const auto promise2 = Promise<std::string>();
  const auto promise3 = Promise<int>();

  auto future1 = promise1.getFuture();
  auto future2 = promise2.getFuture();
  auto future3 = promise3.getFuture();

  const auto comprehendedFuture = ForComprehension(future1,
    [future2]{ return future2; },
    [future3]{ return future3; },
    [](const int& x, const std::string& y, const int& z) { return x + std::stoi(y) + z; }
  );
  EXPECT_TRUE(!comprehendedFuture.isCompleted()) << "Comprehended future should not be completed instantly.";
  promise1.tryComplete(1);
  EXPECT_TRUE(!comprehendedFuture.isCompleted()) << "Comprehended future should not be completed after first promise completion.";
  promise2.tryComplete("2");
  EXPECT_TRUE(!comprehendedFuture.isCompleted()) << "Comprehended future should not be completed after second promise completion.";
  promise3.tryComplete(3);
  EXPECT_TRUE(comprehendedFuture.isCompleted()) << "Comprehended future should be completed after third promise completion.";
  comprehendedFuture.onComplete([](const int& result) {
    EXPECT_EQ(result, 6) << "After completing result should be 6.";
  });
}

TEST(Data_Future, Functionality) {
  const auto promise = Promise<int>();
  const auto future = promise.getFuture();
  EXPECT_TRUE(!future.isCompleted()) << "Future with promise should not be completed instantly.";

  int value = 0;
  future.onComplete([&value](const int& v) { value = v; });
  EXPECT_EQ(value, 0) << "Value should remain 0 after registering on complete.";

  promise.tryComplete(1);
  EXPECT_TRUE(future.isCompleted()) << "Future should be completed after promise completion.";
  EXPECT_EQ(value, 1) << "After completing value should be 1.";

  promise.tryComplete(2);
  EXPECT_EQ(value, 1) << "Value should not change after completing the promise again.";

  int value2 = 0;
  future.onComplete([&value2](const int& v) { value2 = v; });
  EXPECT_EQ(value2, 1) << "Invoking onComplete on successful future should immediately set result.";
}

TEST(Data_Future, Map) {
  const auto promise = Promise<int>();
  const auto future = promise.getFuture().map([](const int& value) {
    return std::to_string(value);
  });
  int value = 0;
  future.onComplete([&value](const std::string& s) { value = std::stoi(s); });
  promise.tryComplete(42);
  EXPECT_TRUE(future.isCompleted()) << "Mapped future should be completed.";
  EXPECT_EQ(value, 42);
}

TEST(Data_Future, FlatMap) {
  const auto promise1 = Promise<int>();
  auto future1 = promise1.getFuture();

  const auto promise2 = Promise<int>();
  auto future2 = promise2.getFuture();

  auto flatMappedFuture = future1.flatMap([&future2](const int& value1) {
    return future2.map([value1](const int& value2) {
      return value1 + value2;
    });
  });
  int value = 0;
  flatMappedFuture.onComplete([&value](const int& v) { value = v; });

  EXPECT_TRUE(!flatMappedFuture.isCompleted()) << "FlatMapped future should not be completed instantly.";

  promise1.tryComplete(1);
  EXPECT_TRUE(!flatMappedFuture.isCompleted()) <<
    "FlatMapped future should not be completed after first promise completion.";

  promise2.tryComplete(2);
  EXPECT_TRUE(flatMappedFuture.isCompleted()) <<
    "FlatMapped future should be completed after second promise completion.";
  EXPECT_EQ(value, 3) << "After completing value should be 3.";
}
