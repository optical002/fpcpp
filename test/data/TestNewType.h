#ifndef FPCPP_TEST_DATA_TEST_NEW_TYPE_H
#define FPCPP_TEST_DATA_TEST_NEW_TYPE_H

#include <core/data/NewType.h>

// TODO look into this std::move solution for this.
GEN_NEW_TYPE(Name, std::string)

GEN_NEW_TYPE_ARITHMETIC(Age, int)

#endif // FPCPP_TEST_DATA_TEST_NEW_TYPE_H