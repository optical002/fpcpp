#pragma once

#include <core/data/Concepts.h>
#include <core/monads/Either.h>

template<PointerType Ptr>
struct SafePtr {
  static Either<std::exception, SafePtr> create(Ptr ptr) {
    return ptr
      ? RightE<std::exception>(SafePtr(ptr))
      : Either<std::exception, SafePtr>::left(std::runtime_error("The pointer was null."));
  }
  // Used for default construction of unused values (e.g. when constructing left for
  // Either<int, SafePtr>).
  SafePtr() : _ptr(nullptr) { }
  
  // ReSharper disable once CppNonExplicitConversionOperator
  operator Ptr() const { return _ptr; }
private:
  explicit SafePtr(Ptr ptr) : _ptr(ptr) {}
  Ptr _ptr;
};

