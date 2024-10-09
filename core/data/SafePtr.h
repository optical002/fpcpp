#ifndef FPCPP_CORE_DATA_SAFE_PTR_H
#define FPCPP_CORE_DATA_SAFE_PTR_H

#include <core/data/Concepts.h>
#include <core/data/Either.h>
#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Eq.h>

template<PointerType Ptr>
struct SafePtr {
  template<PointerType NewA>
  using NewType = SafePtr<NewA>;
  using ValueType = SafePtr;

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

  friend class ToString<SafePtr>;
  friend class Eq<SafePtr>;
};

template<HasToString A>
struct ToString<SafePtr<A>> {
  static std::string toStr(const SafePtr<A>& a) {
    return std::format("SafePtr({})", ToStr(a._ptr));
  }
};

template<HasEq A>
struct Eq<SafePtr<A>> {
  static bool eq(const SafePtr<A>& a, const SafePtr<A>& b) {
    return Equal(a._ptr, b._ptr);
  }
};

#endif // FPCPP_CORE_DATA_SAFE_PTR_H