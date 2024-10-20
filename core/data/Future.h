#ifndef FPCPP_CORE_DATA_FUTURE_H
#define FPCPP_CORE_DATA_FUTURE_H

#include <memory>
#include <functional>
#include <core/data/Option.h>
#include <core/data/Union.h>
#include <core/data/Unit.h>
#include <core/typeclasses/ToString.h>

template <typename A>
class Promise;

template<typename A>
struct AsyncFutureData {
  Option<A> maybeValue = None;
  std::vector<std::function<void(const A&)>> listeners;
};

GEN_UNION_TEMPLATE(FutureImpl, A,
  A, successful,
  std::shared_ptr<AsyncFutureData<A>>, async,
  Unit, unsuccessful
)

/** @brief  Represents a value that will be available in the future. */
template<typename A>
class Future {
public:
  template<typename NewA>
  using NewType = Future<NewA>;
  // TODO add tag and untag;
  using ValueType = A;

private:
  explicit Future(FutureImpl<A> futureImpl) : _futureImpl(std::move(futureImpl)) {}
public:
  static Future successful(const A& value) {
    return Future(FutureImpl<A>::create_successful(value));
  }
  static Future unsuccessful() {
    return Future(FutureImpl<A>::create_unsuccessful({}));
  }
  
  [[nodiscard]] bool isCompleted() const { return _futureImpl.fold(
    [](auto successful) { return true; },
    [](auto async) { return async->maybeValue.isSome(); },
    [](auto unsuccessful) { return false; }
  );}

  [[nodiscard]] FutureImplKind getImplKind() const { return _futureImpl.kind(); }

  void onComplete(const std::function<void(const A&)>& onComplete) const {
    _futureImpl.voidFold(
      [onComplete](auto successful) { onComplete(successful); },
      [onComplete](auto async) {
        async->maybeValue.voidFold(
          [onComplete, async] { async->listeners.push_back(onComplete); },
          [onComplete](const A& value) { onComplete(value); }
        );
      },
      [](auto unsuccessful) { }
    );
  }

  template<
    typename Func,
    typename B = std::invoke_result_t<Func, A>
  > requires std::invocable<Func, A>
  Future<B> map(Func&& f) const {
    auto promise = Promise<B>();

    onComplete([
      promise = std::move(promise), f = std::forward<Func>(f)
    ](const A& value) mutable {
      promise.tryComplete(f(value));
    });

    return promise.getFuture();
  }

  template<
    typename Func,
    typename FutureB = std::invoke_result_t<Func, A>,
    typename B = typename FutureB::ValueType
  > requires std::invocable<Func, A>
  FutureB flatMap(Func&& f) const {
    auto promise = Promise<B>();

    onComplete([
      promise = std::move(promise), f = std::forward<Func>(f)
    ](const A& aValue) mutable {
      f(aValue).onComplete([promise = std::move(promise)](const B& bValue) mutable {
        promise.tryComplete(bValue);
      });
    });

    return promise.getFuture();
  }

private:
  FutureImpl<A> _futureImpl;
  
  friend class Promise<A>;
  friend class ToString<Future>;
};

/** @brief Represents a promise to complete a value in the future. */
template<typename A>
class Promise {
public:
  Promise() : _asyncFuture(
    Future(FutureImpl<A>::create_async(std::make_shared<AsyncFutureData<A>>()))
  ) {}

  Future<A> getFuture() const { return _asyncFuture; }

  void tryComplete(const A& value) const {
    _asyncFuture._futureImpl.voidFold(
      [](auto successful) { },
      [value](auto async) {
        if (async->maybeValue.isSome()) return;
        async->maybeValue = Some(value);

        for (const auto& listener : async->listeners) {
          listener(value);
        }
        async->listeners.clear();
      },
      [](auto unsuccessful) { }
    );
  }
  
private:
  Future<A> _asyncFuture;

  friend class ToString<Promise>;
};

/** @brief Represents a 'Future' which is never completed. */
struct UnsuccessfulType {
  template<typename A>
  // ReSharper disable once CppNonExplicitConversionOperator
  operator Future<A>() const { // NOLINT(*-explicit-constructor)
    return Future<A>::unsuccessful();
  }
};

/** @brief Creates a 'Future' which is completed with 'value'. */
template<typename A>
static Future<A> Successful(const A& value) {
  return Future<A>::successful(value);
}

/** @brief Helper for creating 'UnsuccessfulType' with just typing 'Unsuccessful'. */
static constexpr UnsuccessfulType Unsuccessful{};

template<typename A>
struct ToString<Future<A>> {
  static std::string toStr(const Future<A>& future) {
    return std::format("Future({})", ToStr(future._futureImpl));
  }
};

template<typename A>
struct ToString<AsyncFutureData<A>> {
  static std::string toStr(const AsyncFutureData<A>& asyncFutureData) {
    return std::format(
      "AsyncFutureData(maybeValue={}, listenerCount={})",
      ToStr(asyncFutureData.maybeValue), ToStr(asyncFutureData.listeners.size())
    );
  }
};

template<typename A>
struct ToString<Promise<A>> {
  static std::string toStr(const Promise<A>& promise) {
    return std::format("Promise(asyncFuture={})", ToStr(promise._asyncFuture));
  }
};

#endif // FPCPP_CORE_DATA_FUTURE_H