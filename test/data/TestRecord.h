#ifndef FPCPP_TEST_DATA_TEST_RECORD_H
#define FPCPP_TEST_DATA_TEST_RECORD_H

#include <core/data/Record.h>

GEN_RECORD(TestNoParams)

GEN_RECORD(TestWithParams,
  int, age,
  float, cost,
  std::string, name,
  TestNoParams, testNoParams
)

#endif // FPCPP_TEST_DATA_TEST_RECORD_H