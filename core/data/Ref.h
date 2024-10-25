#ifndef FPCPP_CORE_DATA_REF_H
#define FPCPP_CORE_DATA_REF_H

#include <memory>
#include <core/typeclasses/Str.h>
#include <core/typeclasses/DebugStr.h>

template<typename A>
struct RefData {
  A value;
};

/** @brief A reference to a piece of data allocated on the heap. */
template<typename A>
struct Ref {
  template<typename NewA>
  using NewType = Ref<NewA>;
  using ValueType = A;

  explicit Ref(const A& a) : _data(std::make_shared<RefData<A>>(a)) { }
  
  A getValue() const { return _data->value; }
  void setValue(const A& a) const { _data->value = a; }
private:
  std::shared_ptr<RefData<A>> _data;
};

template<HasStr A>
struct Str<Ref<A>> {
  std::string toStr(const Ref<A>& a) {
    return std::format("Ref({})", ToStr(a));
  }
};

template<HasDebugStr A>
struct DebugStr<Ref<A>> {
  std::string toDebugStr(const Ref<A>& a) {
    return std::format("Ref({})", ToDebugStr(a));
  }
};

#endif // FPCPP_CORE_DATA_REF_H