#ifndef FPCPP_CORE_REACTIVE_OBSERVABLE_H
#define FPCPP_CORE_REACTIVE_OBSERVABLE_H

#include <functional>

#include <core/reactive/Subscribeable.h>
#include <core/reactive/Subscription.h>
#include <core/reactive/Tracker.h>
#include <core/data/Changes.h>
#include <core/data/Ref.h>
#include <core/data/Future.h>

template<typename A>
class Subject;

template<typename A>
class RxVal;

template<typename A>
class RxRef;

template<typename A>
class Observable {
public:
  using ValueType = A;
  
  Subscription subscribe(
    const Tracker& tracker, std::function<void(const A&)> listener
  ) const { return _data.getSubscribeable().subscribe(tracker, listener); }

  template<
    typename Func,
    typename B = std::invoke_result_t<Func, A>
  > requires std::invocable<Func, A>
  Observable<B> map(Func&& f) const;

  template<
    typename Predicate,
    typename FuncResult = std::invoke_result_t<Predicate, A>
  > requires std::invocable<Predicate, A> && std::is_same_v<FuncResult, bool>
  Observable filter(Predicate&& f) const;

  // TODO PS does not work in unreal engine for now
  // template<typename... Observables>
  // requires (std::is_base_of_v<Observable, Observables> && ...)
  // Observable join(Observables... params) const;

  Future<A> toFuture() const;

  Observable<Changes<A>> changes() const;

protected:
  Notifiable<A> _data;

  explicit Observable(Notifiable<A> data) : _data(data) { }
  Observable() = default;

  friend class Subject<A>;
  friend class RxVal<A>;
  friend class RxRef<A>;
};

#include <core/reactive/Subject.h>

template<typename A>
template<typename Func, typename B> requires std::invocable<Func, A>
Observable<B> Observable<A>::map(Func &&f) const {
  auto subject = Subject<B>();
  subscribe(NoOpTracker{}, [
    subject, f = std::forward<Func>(f)
  ](const A& value) {
    subject.push(f(value));
  });

  return subject;
}

template<typename A>
template<
  typename Predicate, typename FuncResult
> requires std::invocable<Predicate, A> && std::is_same_v<FuncResult, bool>
Observable<A> Observable<A>::filter(Predicate &&f) const {
  auto subject = Subject<A>();
  subscribe(NoOpTracker{}, [
    subject, f = std::forward<Predicate>(f)
  ](const A& value) {
    if (f(value)) {
      subject.push(value);
    }
  });
  return subject;
}

//template<typename A>
//template<typename ... Observables> requires (std::is_base_of_v<Observable<A>, Observables> && ...)
//Observable<A> Observable<A>::join(Observables... params) const {
//  std::vector<Observable> observables = {*this, params...};
//  auto subject = Subject<A>();
//  for (auto observable : observables) {
//    observable.subscribe(NoOpTracker{}, [subject](const A& value) {
//      subject.push(value);
//    });
//  }
//  return subject;
//}

template<typename A>
Future<A> Observable<A>::toFuture() const {
  auto promise = Promise<A>();
  auto subscription = subscribe(NoOpTracker{}, [promise](const A& value) {
    promise.tryComplete(value);
  });
  auto future = promise.getFuture();
  future.onComplete([subscription = std::move(subscription)] (A _) {
    subscription.unsubscribe();
  });
  return future;
}

template<typename A>
Observable<Changes<A>> Observable<A>::changes() const {
  auto subject = Subject<Changes<A>>();
  auto maybePrevious = Ref<Option<A>>(None);
  subscribe(NoOpTracker{},
    [subject, maybePrevious](const A& next) {
      maybePrevious.getValue().ifSome([subject, next](const A& previous) {
        subject.push(Changes<A>{previous, next});
      });
      maybePrevious.setValue(Some(next));
    }
  );

  return subject;
}

#endif // FPCPP_CORE_REACTIVE_OBSERVABLE_H
