#ifndef FPCPP_CORE_DATA_CONCEPTS_H
#define FPCPP_CORE_DATA_CONCEPTS_H

#include <type_traits>
#include <memory>

/** @brief A pointer type. */
template<typename T>
concept PointerType = std::is_pointer_v<T>;

/** @brief A smart pointer type. */
template<typename T>
concept SmartPointer = requires {
  typename T::element_type;
} && (
  std::is_same_v<T, std::shared_ptr<typename T::element_type>>
  || std::is_same_v<T, std::unique_ptr<typename T::element_type>>
  || std::is_same_v<T, std::weak_ptr<typename T::element_type>>
);

/** @brief An arithmetic type. */
template<typename A>
concept IsArithmetic = std::is_arithmetic_v<A> && !std::is_same_v<A, bool>;

template<typename FA, typename = void>
struct InnerTypeHelper {
  using Type = typename FA::ValueType;
};

template<typename FA>
struct InnerTypeHelper<FA, std::enable_if_t<SmartPointer<FA>>> {
  using Type = typename FA::element_type::ValueType;
};

/** @brief The inner type of 'FA'. */
template<typename FA>
using InnerType = typename InnerTypeHelper<FA>::Type;

/** @brief Determines if all 'Values' are the same type. */
template<typename... Values>
concept AllSame = (std::is_same_v<std::decay_t<Values>, std::decay_t<std::tuple_element_t<0, std::tuple<Values...>>>> && ...);

/** @brief Determines if 'A' has a nested type 'ValueType'. */
template<typename A>
concept HasValueType = requires { typename A::ValueType; };

/** @brief Determines if 'A' is convertable to 'B'. */
template<typename A, typename B>
concept IsConvertibleTo = std::is_convertible_v<A, B>;

/** @brief Determines if 'B' is convertable from 'A'. */
template<typename A, typename B>
concept IsConvertibleFrom = std::is_convertible_v<B, A>;

/** @brief Annotates a type to be a predicate. */
template<typename Func, typename A>
concept Predicate = std::invocable<Func, A> && std::same_as<bool, std::invoke_result_t<Func, A>>;

/** @brief Annotates a type to be a function. */
template<typename F, typename A, typename Result>
concept Func1 = std::invocable<F, A> && std::same_as<Result, std::invoke_result_t<F, A>>;

/** @brief Annotates a type to be a function with 2 arguments. */
template<typename F, typename A1, typename A2, typename Result>
concept Func2 = std::invocable<F, A1, A2> && std::same_as<Result, std::invoke_result_t<F, A1, A2>>;

template<typename A>
concept IsStringType =
  std::is_same_v<std::decay_t<A>, const char *>
  || std::is_same_v<std::decay_t<A>, char *>
  || std::is_same_v<A, std::string>;

#endif // FPCPP_CORE_DATA_CONCEPTS_H