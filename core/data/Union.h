#ifndef FPCPP_CORE_DATA_UNION_H
#define FPCPP_CORE_DATA_UNION_H

#include <string>
#include <utility>
#include <core/typeclasses/Default.h>
#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Eq.h>
#include <core/data/Option.h>

// TODO finish union generation.
#define GEN_UNION(union_name, ...) \



// GEN_UNION(State, string, name, int, age, float, cost)

enum StateKind : std::uint8_t {
  name = 0, age = 1, cost = 2
};
template<>
struct Eq<StateKind> {
  static bool equal(const StateKind &a, const StateKind &b) { return a == b; }
};
template<>
struct ToString<StateKind> {
  static std::string toStr(const StateKind &a) {
    switch (a) {
      case name: return "name";
      case age: return "age";
      case cost: return "cost";
    }
    throw std::runtime_error("Impossible");
  }
};

struct State {
  const StateKind kind;
private:
  const std::variant<std::string, int, float> _variant;

  State(
    const StateKind& _kind, std::variant<std::string, int, float> v
  ) : kind(_kind), _variant(std::move(v)) {}
public:
  static State create_name(const std::string& name) {
    return {StateKind::name, name};
  }
  static State create_age(const int& age) {
    return {StateKind::age, age};
  }
  static State create_cost(const float& cost) {
    return {StateKind::cost, cost};
  }

  [[nodiscard]] Option<std::string> as_name() const {
    return kind == name ? Some(std::get<0>(_variant)) : None;
  }
  [[nodiscard]] Option<int> as_age() const {
    return kind == age ? Some(std::get<1>(_variant)) : None;
  }
  [[nodiscard]] Option<float> as_cost() const {
    return kind == cost ? Some(std::get<2>(_variant)) : None;
  }

  template<
    typename on_name, typename on_age, typename on_cost,
    typename B = std::invoke_result_t<on_name, std::string>
  >
  requires
    std::invocable<on_name, std::string>
    && std::invocable<on_age, int>
    && std::invocable<on_cost, float>
  B fold(on_name&& _on_name, on_age&& _on_age, on_cost&& _on_cost) const {
    switch (kind) {
      case name: return _on_name(std::get<0>(_variant));
      case age: return _on_age(std::get<1>(_variant));
      case cost: return _on_cost(std::get<2>(_variant));
    }
    throw std::runtime_error("impossible");
  }

  template<typename on_name, typename on_age, typename on_cost>
  requires
    std::invocable<on_name, std::string>
    && std::invocable<on_age, int>
    && std::invocable<on_cost, float>
  void voidFold(on_name&& _on_name, on_age&& _on_age, on_cost&& _on_cost) const {
    switch (kind) {
      case name: _on_name(std::get<0>(_variant)); break;
      case age: _on_age(std::get<1>(_variant)); break;
      case cost: _on_cost(std::get<2>(_variant)); break;
    }
  }
};

template<>
struct Eq<State> {
  static bool equal(const State &a, const State &b) {
    return Equal(a.kind, b.kind) && equalData(a, b);
  }
private:
  static bool equalData(const State &a, const State &b) {
    switch (a.kind) {
      case name: return Equal(a.as_name(), b.as_name());
      case age: return Equal(a.as_age(), b.as_age());
      case cost: return Equal(a.as_cost(), b.as_cost());
    }
    throw std::runtime_error("impossible");
  }
};
template<>
struct ToString<State> {
  static std::string toStr(const State &a) {
    return std::format("State(kind={}, data={})", ToStr(a.kind), toDataString(a));
  }
private:
  static std::string toDataString(const State &a) {
    return a.fold(
      [](const std::string& name) { return ToStr(name); },
      [](const int& age) { return ToStr(age); },
      [](const float& cost) { return ToStr(cost); }
    );
  }
};

#endif // FPCPP_CORE_DATA_UNION_H
