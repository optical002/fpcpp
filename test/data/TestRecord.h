﻿#pragma once
#include <core/data/Record.h>
#include <chaos/preprocessor.h>

GENERATE_RECORD(TestNoParams)

// TODO PS singleton for no params.
// GENERATE_RECORD(TestNoParams)

// GENERATE_RECORD(TestWithParams,
//   int, age,
//   float, cost,
//   std::string, name,
//   TestNoParams, testNoParams
// )
