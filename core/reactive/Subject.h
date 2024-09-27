#ifndef FPCPP_CORE_REACTIVE_SUBJECT_H
#define FPCPP_CORE_REACTIVE_SUBJECT_H

#include <core/reactive/Observable.h>

template<typename A>
class Observable;

template<typename A>
class Subject : public Observable<A> {
public:
  void push(const A& value) const {
    for (const auto& kv : this->_data->listeners) {
      kv.second(value);
    }
  }

  Subject() = default;
};

#endif // FPCPP_CORE_REACTIVE_SUBJECT_H