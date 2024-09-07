#pragma once
#include <core/typeclasses/ToString.h>

struct TestWithToString {};

template<>
struct ToString<TestWithToString> {
  static std::string toStr(const TestWithToString& value) {
    return "TestWithToString()";
  }
};
