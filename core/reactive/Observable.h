#ifndef FPCPP_CORE_REACTIVE_OBSERVABLE_H
#define FPCPP_CORE_REACTIVE_OBSERVABLE_H

#include <functional>
#include <memory>

#include <core/reactive/Subscription.h>
#include <core/reactive/Tracker.h>
#include <core/reactive/Subject.h>
#include <core/data/Changes.h>
#include <core/data/Ref.h>
#include <core/data/Future.h>

template<typename A>
struct ObservableData {
  std::unordered_map<int, std::function<void(const A&)>> listeners;
  int lastListenerIdx = 0;
};

template<typename A>
class Observable {
public:
  using ValueType = A;
  
  Subscription subscribe(
    const Tracker& tracker, std::function<void(const A&)> listener
  ) const {
    auto lastListenerIdx = _data->lastListenerIdx;
    ++_data->lastListenerIdx;
    _data->listeners.insert({lastListenerIdx, listener});
    const auto subscription = Subscription([this, lastListenerIdx] {
      _data->listeners.erase(lastListenerIdx);
    });
    tracker.track(subscription);

    return subscription;
  }

  template<
    typename Func,
    typename B = std::invoke_result_t<Func, A>
  > requires std::invocable<Func, A>
  Observable<B> map(Func&& f) const {
    auto subject = Subject<B>();
    subscribe(NoOpTracker{}, [
      subject, f = std::forward<Func>(f)
    ](const A& value) {
      subject.push(f(value));
    });

    return subject;
  }

  template<
    typename Predicate,
    typename FuncResult = std::invoke_result_t<Predicate, A>
  > requires std::invocable<Predicate, A> && std::is_same_v<FuncResult, bool>
  Observable filter(Predicate&& f) const {
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

  template<typename... Observables>
  requires (std::is_base_of_v<Observable, Observables> && ...)
  Observable join(Observables... params) const {
    std::vector<Observable> observables = {*this, params...};
    auto subject = Subject<A>();
    for (auto observable : observables) {
      observable.subscribe(NoOpTracker{}, [subject](const A& value) {
        subject.push(value);
      });
    }
    return subject;
  }

  Future<A> toFuture() const {
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

  Observable<Changes<A>> changes() const {
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
  
protected:
  std::shared_ptr<ObservableData<A>> _data = std::make_shared<ObservableData<A>>();

  // Disallow creating instances of Observable. It should be done via creating
  // 'Subject' or other type of observable instance.
  Observable() = default;
};

#endif // FPCPP_CORE_REACTIVE_OBSERVABLE_H