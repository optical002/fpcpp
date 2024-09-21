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
class Observable {
public:
  using ValueType = A;
  
  std::weak_ptr<Subscription> subscribe(
    Tracker* tracker, std::function<void(const A&)> observer
  ) {
    auto currentlyAtCopy = _currentlyAt;
    _currentlyAt++;
    _observers.insert({currentlyAtCopy, observer});
    auto subscription = Subscription::create([this, currentlyAtCopy] {
      _observers.erase(currentlyAtCopy);
    });
    tracker->track(subscription);

    return subscription;
  }

  template<
    typename Func,
    typename B = std::invoke_result_t<Func, A>
  > requires std::invocable<Func, A>
  std::shared_ptr<Observable<B>> map(Func&& f) {
    auto subject = std::make_shared<Subject<B>>();
    auto tracker = NoOpTracker{};
    subscribe(&tracker,
      [subject, f = std::forward<Func>(f)](const A& value) {
        subject->push(f(value));
      }
    );

    return subject;
  }

  template<
    typename Predicate,
    typename FuncResult = std::invoke_result_t<Predicate, A>
  > requires std::invocable<Predicate, A> && std::is_same_v<FuncResult, bool>
  std::shared_ptr<Observable> filter(Predicate&& f) {
    auto subject = std::make_shared<Subject<A>>();
    auto tracker = NoOpTracker{};
    subscribe(&tracker,
      [subject, f = std::forward<Predicate>(f)](const A& value) {
        if (f(value)) {
          subject->push(value);
        }
      }
    );
    return subject;
  }

  template<typename... Observables>
  requires (std::is_base_of_v<Observable, Observables> && ...)
  std::shared_ptr<Observable> join(Observables*... params) {
    std::vector<Observable*> observables = {this, params...};
    auto subject = std::make_shared<Subject<A>>();
    for (auto observable : observables) {
      auto tracker = NoOpTracker{};
      observable->subscribe(&tracker, [subject](const A& value) {
        subject->push(value);
      });
    }
    return subject;
  }

  std::shared_ptr<Future<A>> toFuture() {
    auto promise = std::make_shared<Promise<A>>();
    auto tracker = NoOpTracker{};
    auto subscription = subscribe(&tracker,
      [promise](const A& value) {
        promise->tryComplete(value);
      }
    );
    const auto future = promise->getFuture();
    future->onComplete([subscription] (A _) {
      if (auto shared = subscription.lock()) {
        shared->unsubscribe();
      }
    });
    return future;
  }

  std::shared_ptr<Observable<Changes<A>>> changes() {
    auto subject = std::make_shared<Subject<Changes<A>>>();
    auto maybePrevious = Ref<Option<A>>::create(None);
    auto tracker = NoOpTracker{};
    subscribe(&tracker,
      [subject, maybePrevious](const A& next) {
        maybePrevious->value.ifSome([subject, next](const A& previous) {
          subject->push(Changes<A>{previous, next});
        });
        maybePrevious->value = Some(next);
      }
    );

    return subject;
  }
  
protected:
  std::unordered_map<int, std::function<void(const A&)>> _observers;
  int _currentlyAt = 0;

  // Disallow creating instances of Observable. It should be done via creating
  // 'Subject' or other type of observable instance.
  Observable() = default;
};

#endif // FPCPP_CORE_REACTIVE_OBSERVABLE_H