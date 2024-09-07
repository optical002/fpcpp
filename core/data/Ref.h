#pragma once

#include <memory>

template<typename A>
struct Ref {
  static std::shared_ptr<Ref> create(const A& a) { return std::make_shared<Ref>(a); }
  explicit Ref(const A& a) : value(a) { }
  
  A value;
};