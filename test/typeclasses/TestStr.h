#ifndef FPCPP_TEST_TYPECLASSES_TEST_STR_H
#define FPCPP_TEST_TYPECLASSES_TEST_STR_H

#include <core/typeclasses/Str.h>

struct TestWithStr {};

template<>
struct Str<TestWithStr> {
  static std::string toStr(const TestWithStr& value) {
    return "TestWithStr()";
  }
};

#endif // FPCPP_TEST_TYPECLASSES_TEST_STR_H