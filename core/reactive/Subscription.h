#ifndef FPCPP_CORE_REACTIVE_SUBSCRIPTION_H
#define FPCPP_CORE_REACTIVE_SUBSCRIPTION_H

#include <functional>
#include <memory>

struct SubscriptionData {
  std::function<void()> unsubscribe;
  bool isSubscribed;

  SubscriptionData(
    std::function<void()> unsubscribe_, const bool isSubscribed_
  ) : unsubscribe(std::move(unsubscribe_)), isSubscribed(isSubscribed_) { }
};

class Subscription {
public:
  explicit Subscription(
    std::function<void()> unsubscribe
  ) : _data(std::make_shared<SubscriptionData>(std::move(unsubscribe), true)) { }

  bool unsubscribe() const {
    if (_data->isSubscribed) {
      _data->unsubscribe();
      _data->isSubscribed = false;
      return true;
    } else {
      return false;
    }
  }

private:
  std::shared_ptr<SubscriptionData> _data;
};

#endif // FPCPP_CORE_REACTIVE_SUBSCRIPTION_H