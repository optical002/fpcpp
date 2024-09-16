﻿#pragma once

#include <concepts>
#include <string>
#include <core/data/Concepts.h>

template<typename A>
struct Semigroup;

template<typename A>
concept HasSemigroup = requires(const A& a, const A& b) {
  { Semigroup<A>::combine(a, b) } -> std::same_as<A>;
};

template<HasSemigroup A>
A Combine(const A& a, const A& b) {
  return Semigroup<A>::combine(a, b);
};

template<>
struct Semigroup<std::string> {
  static std::string combine(const std::string& a, const std::string& b) {
    return a + b;
  }
};

template<IsArithmetic A>
struct Semigroup<A> {
  static A combine(const A& a, const A& b) {
    return a + b; 
  }
};
