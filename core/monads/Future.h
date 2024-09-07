#pragma once

#include <memory>
#include <functional>
#include "Option.h"

template <typename A>
class Promise;

/**
 * Represents a value that will be available in the future.
 */
template<typename A>
class Future {
public:
  using ValueType = A;
  
  static std::shared_ptr<Future> successful(const A& value) {
    return std::make_shared<Future>(value);
  }
  static std::shared_ptr<Future> unsuccessful() {
    return std::make_shared<Future>();
  }
  
  explicit Future(const A& a) : _maybeValue(Some(a)) {}
  Future() : _maybeValue(None) {}  // NOLINT(modernize-use-equals-default)

  bool isCompleted() const { return _maybeValue.isSome(); }

  void onComplete(const std::function<void(const A&)> onComplete) {
    _maybeValue.voidFold(
      [this, onComplete] { _callbacks.push_back(onComplete); },
      [onComplete](const A& value) { onComplete(value);}
    );
  }

  template<
    typename Func,
    typename B = std::invoke_result_t<Func, A>
  > requires std::invocable<Func, A>
  std::shared_ptr<Future<B>> map(Func&& f) {
    const auto promise = Promise<B>();

    onComplete([promise, f = std::forward<Func>(f)](const A& value) {
      promise.tryComplete(f(value));
    });

    return promise.getFuture();
  }

  template<
    typename Func,
    typename PtrFutureB = std::invoke_result_t<Func, A>,
    typename FutureB = typename PtrFutureB::element_type,
    typename B = typename FutureB::ValueType
  > requires std::invocable<Func, A>
  PtrFutureB flatMap(Func&& f) {
    const auto promise = Promise<B>();

    onComplete([promise, f = std::forward<Func>(f)](const A& aValue) {
      f(aValue)->onComplete([promise](const B& bValue) {
        promise.tryComplete(bValue);
      });
    });

    return promise.getFuture();
  }

private:
  void tryComplete(const A& value) {
    if (_maybeValue.isSome()) return;

    _maybeValue = Some(value);

    for (const auto& callback : _callbacks) {
      callback(value);
    }

    _callbacks.clear();
  }
  
  Option<A> _maybeValue;
  std::vector<std::function<void(const A&)>> _callbacks;
  
  friend class Promise<A>;
};

template<typename A>
class Promise {
public:
  Promise() : _future(std::make_shared<Future<A>>()) {}  // NOLINT(modernize-use-equals-default)

  std::shared_ptr<Future<A>> getFuture() const { return _future; }

  void tryComplete(const A& value) const {
    _future->tryComplete(value);
  }
  
private:
  std::shared_ptr<Future<A>> _future;
};

struct UnsuccessfulType {
  template<typename A>
  // ReSharper disable once CppNonExplicitConversionOperator
  operator std::shared_ptr<Future<A>>() const {
    return Future<A>::unsuccessful();
  }
};

class FutureA {
public:
  template<typename A>
  static std::shared_ptr<Future<A>> successful(const A& value) {
    return Future<A>::successful(value);
  }

  static constexpr UnsuccessfulType unsuccessful{};
};