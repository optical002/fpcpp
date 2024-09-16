#pragma once
#include <core/data/Record.h>

GEN_RECORD(TestNoParams)

GEN_RECORD(TestWithParams,
  int, age,
  float, cost,
  std::string, name,
  TestNoParams, testNoParams
)
