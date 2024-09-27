#ifndef FPCPP_CORE_REACTIVE_TRACKER_H
#define FPCPP_CORE_REACTIVE_TRACKER_H

#include <memory>
#include <core/reactive/Subscription.h>

class Tracker {
public:
  void track(const Subscription& subscription) const {
    trackBase(subscription);
  }
  
  void track(const std::function<void()>& action) const {
    track(Subscription(action));
  }

protected:
  virtual void trackBase(const Subscription& subscription) const = 0;
  
  Tracker() = default;
  virtual ~Tracker() = default;
};

class NoOpTracker final : public Tracker {
  void trackBase(const Subscription& subscription) const override { }
};

struct DisposableTrackerData {
  std::vector<Subscription> subscriptions;
};

class DisposableTracker final : public Tracker {
public:
  DisposableTracker() : _data(std::make_shared<DisposableTrackerData>()) {}

  // Do not leave active subscriptions when the tracker is destroyed.
  ~DisposableTracker() override { dispose(); }

  void dispose() const {
    for (const auto& sub : _data->subscriptions) {
      // ReSharper disable once CppExpressionWithoutSideEffects
      sub.unsubscribe();
    }
    _data->subscriptions.clear();
  }

private:
  void trackBase(const Subscription& subscription) const override {
    _data->subscriptions.push_back(subscription);
  }

  std::shared_ptr<DisposableTrackerData> _data;
};

#endif // FPCPP_CORE_REACTIVE_TRACKER_H