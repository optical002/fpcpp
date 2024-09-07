#pragma once

#include <format>
#include <string>
#include <stdexcept>
#include "core/monads/Either.h"
#include "core/data/Unit.h"
#include "core/data/Concepts.h"
#include "core/data/SafePtr.h"

/**
 * Tries to evaluate 'Func' expression and returns the result as an 'Either' type.
 * @note Does not catch if pointer is 'nullptr', since in c++ it is not an exception.
 */
template<
  typename Func,
  typename FuncReturn = decltype(std::declval<Func>()()),
  typename IsReturnVoid = std::is_void<FuncReturn>,
  typename Right = std::conditional_t<IsReturnVoid::value, Unit, FuncReturn>,
  typename TryType = Either<std::exception, Right>
> requires std::invocable<Func>
TryType Try(Func&& f) {
  try {
    if constexpr (IsReturnVoid::value) {
      f();
      return TryType::right(Unit());
    } else {
      return TryType::right(f());
    }
  } catch (const std::exception& e) {
    return TryType::left(e);
  } catch (...) {
    return TryType::left(std::runtime_error("Unknown exception"));
  }
}

/**
 * Tries to evaluate 'Func' expression if successful sets 'val' to the result, else logs the error.
 */
template<
  typename Func, typename Log, typename A,
  typename FuncReturn = decltype(std::declval<Func>()()),
  typename IsReturnVoid = std::is_void<FuncReturn>
> requires
  std::invocable<Func>
  && std::invocable<Log, std::string>
  && (IsReturnVoid::value || std::is_same_v<FuncReturn, A>)
// ReSharper disable once CppInconsistentNaming
void TryLogSet(Func&& tryVal, A& val, Log&& log) {
  if constexpr (IsReturnVoid::value) {
    Try(tryVal).left().ifSome([&log](const std::exception& e) {
      log(std::format("Encountered error in 'Try' - {}", e.what()));
    });
  } else {
    Try(tryVal).voidFold(
      [&log](const std::exception& e) {
        log(std::format("Encountered error in 'Try' - {}", e.what()));
      },
      [&val](const A& v) { val = v; }
    );
  }
}

template<PointerType Ptr>
// ReSharper disable once CppInconsistentNaming
Either<std::exception, SafePtr<Ptr>> TryPtr(Ptr ptr) {
  return SafePtr<Ptr>::create(ptr);
}
