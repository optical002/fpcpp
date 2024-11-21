#ifndef FPCPP_CORE_DATA_NON_EMPTY_H
#define FPCPP_CORE_DATA_NON_EMPTY_H

#include <core/data/Option.h>

template<typename C>
concept IsCollection = requires {
  typename C::ValueType;
  { std::declval<C>().isEmpty() } -> std::same_as<bool>;
  // TODO Add: begin and end iterators.
};

// TODO Finish
template<IsCollection C>
struct NonEmpty {
public:
  static Option<C> create(const C& collection) {
    return collection.isEmpty() ? None : Some(collection);
  }

  C a() const { return _collection; }

private:
  C _collection;
};

#endif //FPCPP_CORE_DATA_NON_EMPTY_H
