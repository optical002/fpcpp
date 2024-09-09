#pragma once
#include <type_traits>
#include <memory>

template<typename T>
concept PointerType = std::is_pointer_v<T>;

template<typename T>
concept SmartPointer = requires {
  typename T::element_type;
} && (
  std::is_same_v<T, std::shared_ptr<typename T::element_type>>
  || std::is_same_v<T, std::unique_ptr<typename T::element_type>>
  || std::is_same_v<T, std::weak_ptr<typename T::element_type>>
);

template<typename FA, typename = void>
struct InnerTypeHelper {
  using Type = typename FA::ValueType;
};

template<typename FA>
struct InnerTypeHelper<FA, std::enable_if_t<SmartPointer<FA>>> {
  using Type = typename FA::element_type::ValueType;
};

template<typename FA>
using InnerType = typename InnerTypeHelper<FA>::Type;