#ifndef FPCPP_CORE_REACTIVE_SUBJECT_H
#define FPCPP_CORE_REACTIVE_SUBJECT_H

template<typename A>
class Observable;

template<typename A>
class Subject : public Observable<A> {
public:
  void push(const A& value) const;

  Subject() = default;
};

// ReSharper disable once CppUnusedIncludeDirective
#include <core/reactive/Observable.h>

template<typename A>
void Subject<A>::push(const A &value) const {
  this->_data.getPushable().push(value);
}
#endif // FPCPP_CORE_REACTIVE_SUBJECT_H
