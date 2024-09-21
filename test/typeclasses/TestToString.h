#ifndef FPCPP_TEST_TYPECLASSES_TEST_TO_STRING_H
#define FPCPP_TEST_TYPECLASSES_TEST_TO_STRING_H

#include <core/typeclasses/ToString.h>

struct TestWithToString {};

template<>
struct ToString<TestWithToString> {
  static std::string toStr(const TestWithToString& value) {
    return "TestWithToString()";
  }
};

#endif // FPCPP_TEST_TYPECLASSES_TEST_TO_STRING_H