#ifndef FPCPP_CORE_REACTIVE_RX_REF_H
#define FPCPP_CORE_REACTIVE_RX_REF_H

#include <core/data/Ref.h>
#include <core/reactive/Subscription.h>
#include <core/reactive/Tracker.h>
#include <core/reactive/Observable.h>

template<typename A>
struct RxRef {
  using ValueType = A;

private:
  Ref<A> _ref;
};

struct RxVal {

};

#endif //FPCPP_CORE_REACTIVE_RX_REF_H
