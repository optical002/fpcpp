#ifndef FPCPP_TEST_DATA_TEST_UNION_H
#define FPCPP_TEST_DATA_TEST_UNION_H

#include <core/data/Union.h>

GEN_UNION(State, std::string, name, int, age, float, cost)

// TODO test GEN_UNION_TEMPLATE

#endif //FPCPP_TEST_DATA_TEST_UNION_H
