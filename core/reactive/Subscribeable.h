#ifndef FPCPP_CORE_REACTIVE_SUBSCRIBEABLE_H
#define FPCPP_CORE_REACTIVE_SUBSCRIBEABLE_H

#include <functional>
#include <memory>

#include <core/data/Ref.h>
#include <core/reactive/Subscription.h>
#include <core/reactive/Tracker.h>

template<typename A>
class Notifiable;

template<typename A>
struct SubscribeableData {
  std::unordered_map<int, std::function<void(const A&)>> listeners;
  int lastListenerIdx = 0;
};

/** @brief A Pushable is a type which can push a value to something that has subscribed. */
template<typename A>
class Pushable {
public:
  using ValueType = A;

  void push(const A& value) const {
    for (const auto& kv : _data.getValue().listeners) {
      kv.second(value);
    }
  }

private:
  Ref<SubscribeableData<A>> _data;

  explicit Pushable(Ref<SubscribeableData<A>> data) : _data(data) { }

  friend class Notifiable<A>;
};

/** @brief A Subscribeable is a type that can be subscribed to. */
template<typename A>
class Subscribeable {
public:
  using ValueType = A;

  Subscription subscribe(
    const Tracker& tracker, std::function<void(const A&)> listener
  ) const {
    auto data = _data.getSharedPtr();

    auto lastListenerIdx = data->lastListenerIdx;
    ++data->lastListenerIdx;
    data->listeners.insert({lastListenerIdx, listener});
    const auto subscription = Subscription([data, lastListenerIdx] {
      data->listeners.erase(lastListenerIdx);
    });
    tracker.track(subscription);

    return subscription;
  }

private:
  Ref<SubscribeableData<A>> _data;

  explicit Subscribeable(Ref<SubscribeableData<A>> data) : _data(data) { }

  friend class Notifiable<A>;
};

template<typename A>
class Notifiable {
public:
  using ValueType = A;

  Notifiable() = default;

  Pushable<A> getPushable() const { return Pushable<A>(_data); }
  Subscribeable<A> getSubscribeable() const { return Subscribeable<A>(_data); }

private:
  Ref<SubscribeableData<A>> _data;
};

#endif //FPCPP_CORE_REACTIVE_SUBSCRIBEABLE_H
