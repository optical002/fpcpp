#pragma once

#include "Observable.h"

template<typename A>
class Observable;

template<typename A>
class Subject : public Observable<A> {
public:
  void push(const A& value) {
    for (const auto& kv : this->_observers) {
      kv.second(value);
    }
  }

  Subject() = default;
};