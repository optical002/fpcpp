#ifndef FPCPP_CORE_DATA_REF_H
#define FPCPP_CORE_DATA_REF_H

#include <memory>

template<typename A>
struct RefData {
  A value;
};

template<typename A>
struct Ref {
  explicit Ref(const A& a) : _data(std::make_shared<RefData<A>>(a)) { }
  
  A getValue() const { return _data->value; }
  void setValue(const A& a) const { _data->value = a; }
private:
  std::shared_ptr<RefData<A>> _data;
};

#endif // FPCPP_CORE_DATA_REF_H