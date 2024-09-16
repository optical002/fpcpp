#pragma once
#include <core/typeclasses/Num.h>

class TestWithNum {};

template<>
struct Num<TestWithNum> {
  static TestWithNum add(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum subtract(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum multiply(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum divide(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
  static TestWithNum mod(const TestWithNum& t1, const TestWithNum& t2) { return t1; }
};