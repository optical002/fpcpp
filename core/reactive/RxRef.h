#ifndef FPCPP_CORE_REACTIVE_RX_REF_H
#define FPCPP_CORE_REACTIVE_RX_REF_H

#include <core/data/Ref.h>
#include <core/data/Enums.h>
#include <core/reactive/Subscription.h>
#include <core/reactive/Tracker.h>
#include <core/reactive/Observable.h>
#include <core/reactive/Subscribeable.h>

template<typename A>
class RxRef;

template<typename A>
class RxVal {
public:
  using ValueType = A;

  static RxVal a(const A a) { return RxVal(Notifiable<A>(), Ref<A>(a)); }

  Subscription subscribe(
    const Tracker& tracker, std::function<void(const A&)> listener,
    SubscribeBehaviour behaviour = SubscribeBehaviour::InvokeOnSubscribe
  ) const {
    auto sub = _notifiable.getSubscribeable().subscribe(tracker, listener);
    if (behaviour == SubscribeBehaviour::InvokeOnSubscribe) {
      _notifiable.getPushable().push(_ref.getValue());
    }
    return sub;
  }

  A getValue() const { return _ref.getValue(); }

  operator Observable<A>() const { return Observable<A>(_notifiable); }

  template<
    typename Func, typename B = std::invoke_result_t<Func, A>
  > requires std::invocable<Func, A>
  RxVal<B> map(Func&& f) const;

  template<
    typename Func,
    typename RxValB = std::invoke_result_t<Func, A>,
    typename B = typename RxValB::ValueType
  > requires std::invocable<Func, A>
  RxVal<B> flatMap(Func&& f) const;

private:
  Ref<A> _ref;
  Notifiable<A> _notifiable;

  RxVal(
    const Notifiable<A> notifiable, const Ref<A> ref
  ) : _notifiable(notifiable), _ref(ref)  { }

  friend class RxRef<A>;
};

template<typename A>
class RxRef {
public:
  using ValueType = A;

  static RxRef a(const A a) { return RxRef(Ref<A>(a)); }

  Subscription subscribe(
    const Tracker& tracker, std::function<void(const A&)> listener,
    SubscribeBehaviour behaviour = SubscribeBehaviour::InvokeOnSubscribe
  ) const {
    auto sub = _notifiable.getSubscribeable().subscribe(tracker, listener);
    if (behaviour == SubscribeBehaviour::InvokeOnSubscribe) {
      _notifiable.getPushable().push(_ref.getValue());
    }
    return sub;
  }

  A getValue() const { return _ref.getValue(); }
  void setValue(const A a) const {
    _ref.unsafe_setValueMove(a);
    _notifiable.getPushable().push(a);
  }

  operator RxVal<A>() const { return RxVal<A>(_notifiable, _ref); }
  operator Observable<A>() const { return Observable<A>(_notifiable); }

  template<
    typename Func, typename B = std::invoke_result_t<Func, A>
  > requires std::invocable<Func, A>
  RxVal<B> map(Func&& f) const {
    RxVal<A> rxVal = *this;
    return rxVal.map(f);
  }

  template<
    typename Func,
    typename RxValB = std::invoke_result_t<Func, A>,
    typename B = typename RxValB::ValueType
  > requires std::invocable<Func, A>
  RxVal<B> flatMap(Func&& f) const {
    RxVal<A> rxVal = *this;
    return rxVal.flatMap(f);
  }

private:
  Ref<A> _ref;
  Notifiable<A> _notifiable = Notifiable<A>();

  explicit RxRef(const Ref<A> ref) : _ref(ref) { }
};

template<typename A>
template<typename Func, typename B> requires std::invocable<Func, A>
RxVal<B> RxVal<A>::map(Func&& f) const {
  auto rxRef = RxRef<B>::a(f(_ref.getValue()));
  subscribe(NoOpTracker{}, [
    rxRef, f = std::forward<Func>(f)
  ](const A& value) {
    rxRef.setValue(f(value));
  }, SubscribeBehaviour::InvokeOnNextVal);

  return rxRef;
}

template<typename A>
template<typename Func, typename RxValB, typename B> requires std::invocable<Func, A>
RxVal<B> RxVal<A>::flatMap(Func&& f) const {
  auto rxRefResult = RxRef<B>::a(f(_ref.getValue()).getValue());
  auto disposableTracker = DisposableTracker();

  subscribe(NoOpTracker{}, [
    rxRefResult, disposableTracker, f = std::forward<Func>(f)
  ](const A& value) {
    disposableTracker.dispose();
    auto innerRx = f(value);
    innerRx.subscribe(disposableTracker, [rxRefResult](const B& value) {
      rxRefResult.setValue(value);
    });
  }, SubscribeBehaviour::InvokeOnNextVal);

  return rxRefResult;
}

#endif //FPCPP_CORE_REACTIVE_RX_REF_H
