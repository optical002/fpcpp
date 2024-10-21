#ifndef FPCPP_TEST_TYPECLASSES_TESTDEBUGSTR_H
#define FPCPP_TEST_TYPECLASSES_TESTDEBUGSTR_H

#include <core/typeclasses/DebugStr.h>

struct TestWithDebugStr {};

template<>
struct DebugStr<TestWithDebugStr> {
  static std::string toDebugStr(const TestWithDebugStr& value) {
    return "TestWithDebugStr()";
  }
};

#endif //FPCPP_TEST_TYPECLASSES_TESTDEBUGSTR_H
