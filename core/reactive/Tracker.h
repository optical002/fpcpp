#pragma once

#include <memory>
#include <mutex>

#include "Subscription.h"

class Tracker {
public:
  void track(const std::shared_ptr<Subscription>& subscription) {
    trackBase(subscription);
  }
  
  void track(const std::function<void()>& action) {
    track(Subscription::create(action));
  }

protected:
  virtual void trackBase(const std::shared_ptr<Subscription>& subscription) = 0;
  
  Tracker() = default;
  virtual ~Tracker() = default;
};

class NoOpTracker final : public Tracker {
public:
  NoOpTracker(const NoOpTracker&) = delete;
  NoOpTracker& operator=(const NoOpTracker&) = delete;

  static NoOpTracker* instance() {
    std::call_once(_initFlag, [] {
      _instance.reset(new NoOpTracker);
    });
    return _instance.get();
  }

private:
  void trackBase(const std::shared_ptr<Subscription>& subscription) override { }

  NoOpTracker() = default;

  static std::unique_ptr<NoOpTracker> _instance;
  static std::once_flag _initFlag;
};

class DisposableTracker final : public Tracker {
public:
  DisposableTracker() = default;

  // Do not leave active subscriptions when the tracker is destroyed.
  ~DisposableTracker() override { dispose();	}

  void dispose() {
    for (const auto& sub : _subscriptions) {
      sub->unsubscribe();
    }
    _subscriptions.clear();
  }

private:
  void trackBase(const std::shared_ptr<Subscription>& subscription) override {
    _subscriptions.push_back(subscription);
  }

  std::vector<std::shared_ptr<Subscription>> _subscriptions;
};
