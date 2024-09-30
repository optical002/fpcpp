#ifndef FPCPP_CORE_DATA_CONCEPTS_H
#define FPCPP_CORE_DATA_CONCEPTS_H

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

template<typename A>
concept IsArithmetic = std::is_arithmetic_v<A>;

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

template<typename... Values>
concept AllSame = (std::is_same_v<std::decay_t<Values>, std::decay_t<std::tuple_element_t<0, std::tuple<Values...>>>> && ...);

template<typename A>
concept HasValueType = requires { typename A::ValueType; };

template<typename A, typename B>
concept IsConvertibleTo = std::is_convertible_v<A, B>;

template<typename A, typename B>
concept IsConvertibleFrom = std::is_convertible_v<B, A>;

template<typename Func, typename A>
concept Predicate = std::invocable<Func, A> && std::same_as<bool, std::invoke_result_t<Func, A>>;

template<typename F, typename A, typename Result>
concept Func = std::invocable<F, A> && std::same_as<Result, std::invoke_result_t<F, A>>;

#endif // FPCPP_CORE_DATA_CONCEPTS_H