#pragma once

#include <functional>
#include <memory>

class Subscription {
public:
  static std::shared_ptr<Subscription> create(
    const std::function<void()>& unsubscribe
  ) {
    return std::make_shared<Subscription>(unsubscribe);
  }

  explicit Subscription(
    const std::function<void()>& unsubscribe
  ) : _unsubscribe(unsubscribe), _isSubscribed(true) { }

  bool unsubscribe() {
    if (_isSubscribed) {
      _unsubscribe();
      _isSubscribed = false;
      return true;
    } else {
      return false;
    }
  }

private:
  std::function<void()> _unsubscribe;
  bool _isSubscribed;
};
