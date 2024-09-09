#pragma once
#include <concepts>

#include "core/data/Concepts.h"

template<typename FA, typename Map>
requires std::invocable<Map, InnerType<FA>>
auto ForComprehension(FA fa, Map&& f) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return fa->map(std::forward<Map>(f));
  } else {
    return fa.map(std::forward<Map>(f));
  }
}

// For comprehension for chaining (e.g. fa.flatMap(flatMap).flatMap(flatMap2).map(map))
template<
  typename FA,  typename FlatMap, typename... Tail
> requires std::invocable<FlatMap, InnerType<FA>>
auto ForComprehension(
  FA fa, FlatMap&& flatMap, Tail&&... tail
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return ForComprehension(
      fa->flatMap(std::forward<FlatMap>(flatMap)),
      std::forward<Tail>(tail)...
    );
  } else {
    return ForComprehension(
      fa.flatMap(std::forward<FlatMap>(flatMap)),
      std::forward<Tail>(tail)...
    ); 
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap, typename FlatMapResult = InnerType<std::invoke_result_t<FlatMap, A>>,
  typename Map
> requires
  std::invocable<FlatMap, A>
  && std::invocable<Map, A, FlatMapResult>
auto ForComprehension(
  FA fa, FlatMap&& flatMap, Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap(a)->map([=](FlatMapResult flatMapResult) { return
      map(a, flatMapResult);
      }); });  
  } else {
    return 
      fa.flatMap([=](A a) { return
      flatMap(a).map([=](FlatMapResult flatMapResult) { return
      map(a, flatMapResult);
      }); });
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->map([=](FlatMap2Result flatMap2Result) { return
      map(a, flatMap1Result, flatMap2Result);
      }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).map([=](FlatMap2Result flatMap2Result) { return
      map(a, flatMap1Result, flatMap2Result);
      }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3, A, FlatMap1Result, FlatMap2Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<FlatMap3, A, FlatMap1Result, FlatMap2Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result)->map([=](FlatMap3Result flatMap3Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result);
      }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result).map([=](FlatMap3Result flatMap3Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result);
      }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3, A, FlatMap1Result, FlatMap2Result>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<FlatMap3, A, FlatMap1Result, FlatMap2Result>
  && std::invocable<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result)->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result)->map([=](FlatMap4Result flatMap4Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result);
      }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result).flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result).map([=](FlatMap4Result flatMap4Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result);
      }); }); }); }); });
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3, A, FlatMap1Result, FlatMap2Result>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<FlatMap3, A, FlatMap1Result, FlatMap2Result>
  && std::invocable<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
  && std::invocable<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result)->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result)->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result)->map([=](FlatMap5Result flatMap5Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result);
      }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result).flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result).flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result).map([=](FlatMap5Result flatMap5Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result);
      }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3, A, FlatMap1Result, FlatMap2Result>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<FlatMap3, A, FlatMap1Result, FlatMap2Result>
  && std::invocable<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
  && std::invocable<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>
  && std::invocable<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result)->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result)->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result)->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result)->map([=](FlatMap6Result flatMap6Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result);
      }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result).flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result).flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result).flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result).map([=](FlatMap6Result flatMap6Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result);
      }); }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3, A, FlatMap1Result, FlatMap2Result>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>>,
  typename FlatMap7, typename FlatMap7Result = InnerType<std::invoke_result_t<FlatMap7, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<FlatMap3, A, FlatMap1Result, FlatMap2Result>
  && std::invocable<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
  && std::invocable<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>
  && std::invocable<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>
  && std::invocable<FlatMap7, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  FlatMap7&& flatMap7,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result)->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result)->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result)->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result)->flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result)->map([=](FlatMap7Result flatMap7Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result);
      }); }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result).flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result).flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result).flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result).flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result).map([=](FlatMap7Result flatMap7Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result);
      }); }); }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3, A, FlatMap1Result, FlatMap2Result>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>>,
  typename FlatMap7, typename FlatMap7Result = InnerType<std::invoke_result_t<FlatMap7, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>>,
  typename FlatMap8, typename FlatMap8Result = InnerType<std::invoke_result_t<FlatMap8, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<FlatMap3, A, FlatMap1Result, FlatMap2Result>
  && std::invocable<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
  && std::invocable<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>
  && std::invocable<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>
  && std::invocable<FlatMap7, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>
  && std::invocable<FlatMap8, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result, FlatMap8Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  FlatMap7&& flatMap7,
  FlatMap8&& flatMap8,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result)->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result)->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result)->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result)->flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result)->flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result)->map([=](FlatMap8Result flatMap8Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result);
      }); }); }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result).flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result).flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result).flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result).flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result).flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result).map([=](FlatMap8Result flatMap8Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result);
      }); }); }); }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1, A>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2, A, FlatMap1Result>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3, A, FlatMap1Result, FlatMap2Result>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>>,
  typename FlatMap7, typename FlatMap7Result = InnerType<std::invoke_result_t<FlatMap7, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>>,
  typename FlatMap8, typename FlatMap8Result = InnerType<std::invoke_result_t<FlatMap8, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result>>,
  typename FlatMap9, typename FlatMap9Result = InnerType<std::invoke_result_t<FlatMap9, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result, FlatMap8Result>>,
  typename Map
> requires
  std::invocable<FlatMap1, A>
  && std::invocable<FlatMap2, A, FlatMap1Result>
  && std::invocable<FlatMap3, A, FlatMap1Result, FlatMap2Result>
  && std::invocable<FlatMap4, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
  && std::invocable<FlatMap5, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>
  && std::invocable<FlatMap6, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>
  && std::invocable<FlatMap7, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>
  && std::invocable<FlatMap8, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result>
  && std::invocable<FlatMap9, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result, FlatMap8Result>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result, FlatMap8Result, FlatMap9Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  FlatMap7&& flatMap7,
  FlatMap8&& flatMap8,
  FlatMap9&& flatMap9,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1(a)->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result)->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result)->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result)->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result)->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result)->flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result)->flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result)->flatMap([=](FlatMap8Result flatMap8Result) { return
      flatMap9(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result)->map([=](FlatMap9Result flatMap9Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result, flatMap9Result);
      }); }); }); }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1(a).flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2(a, flatMap1Result).flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3(a, flatMap1Result, flatMap2Result).flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4(a, flatMap1Result, flatMap2Result, flatMap3Result).flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result).flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result).flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result).flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result).flatMap([=](FlatMap8Result flatMap8Result) { return
      flatMap9(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result).map([=](FlatMap9Result flatMap9Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result, flatMap9Result);
      }); }); }); }); }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap, typename FlatMapResult = InnerType<std::invoke_result_t<FlatMap>>,
  typename Map
> requires
  std::invocable<FlatMap>
  && std::invocable<Map, A, FlatMapResult>
auto ForComprehension(
  FA fa, FlatMap&& flatMap, Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap()->map([=](FlatMapResult flatMapResult) { return
      map(a, flatMapResult);
      }); });  
  } else {
    return 
      fa.flatMap([=](A a) { return
      flatMap().map([=](FlatMapResult flatMapResult) { return
      map(a, flatMapResult);
      }); });
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->map([=](FlatMap2Result flatMap2Result) { return
      map(a, flatMap1Result, flatMap2Result);
      }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().map([=](FlatMap2Result flatMap2Result) { return
      map(a, flatMap1Result, flatMap2Result);
      }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<FlatMap3>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3()->map([=](FlatMap3Result flatMap3Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result);
      }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3().map([=](FlatMap3Result flatMap3Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result);
      }); }); }); });  
  }
}


template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<FlatMap3>
  && std::invocable<FlatMap4>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3()->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4()->map([=](FlatMap4Result flatMap4Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result);
      }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3().flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4().map([=](FlatMap4Result flatMap4Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result);
      }); }); }); }); });
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<FlatMap3>
  && std::invocable<FlatMap4>
  && std::invocable<FlatMap5>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3()->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4()->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5()->map([=](FlatMap5Result flatMap5Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result);
      }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3().flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4().flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5().map([=](FlatMap5Result flatMap5Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result);
      }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<FlatMap3>
  && std::invocable<FlatMap4>
  && std::invocable<FlatMap5>
  && std::invocable<FlatMap6>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3()->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4()->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5()->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6()->map([=](FlatMap6Result flatMap6Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result);
      }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3().flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4().flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5().flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6().map([=](FlatMap6Result flatMap6Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result);
      }); }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6>>,
  typename FlatMap7, typename FlatMap7Result = InnerType<std::invoke_result_t<FlatMap7>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<FlatMap3>
  && std::invocable<FlatMap4>
  && std::invocable<FlatMap5>
  && std::invocable<FlatMap6>
  && std::invocable<FlatMap7>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  FlatMap7&& flatMap7,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3()->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4()->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5()->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6()->flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7()->map([=](FlatMap7Result flatMap7Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result);
      }); }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3().flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4().flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5().flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6().flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7().map([=](FlatMap7Result flatMap7Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result);
      }); }); }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6>>,
  typename FlatMap7, typename FlatMap7Result = InnerType<std::invoke_result_t<FlatMap7>>,
  typename FlatMap8, typename FlatMap8Result = InnerType<std::invoke_result_t<FlatMap8>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<FlatMap3>
  && std::invocable<FlatMap4>
  && std::invocable<FlatMap5>
  && std::invocable<FlatMap6>
  && std::invocable<FlatMap7>
  && std::invocable<FlatMap8>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result, FlatMap8Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  FlatMap7&& flatMap7,
  FlatMap8&& flatMap8,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3()->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4()->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5()->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6()->flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7()->flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8()->map([=](FlatMap8Result flatMap8Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result);
      }); }); }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3().flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4().flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5().flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6().flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7().flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8().map([=](FlatMap8Result flatMap8Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result);
      }); }); }); }); }); }); }); }); });  
  }
}

template<
  typename FA, typename A = InnerType<FA>,
  typename FlatMap1, typename FlatMap1Result = InnerType<std::invoke_result_t<FlatMap1>>,
  typename FlatMap2, typename FlatMap2Result = InnerType<std::invoke_result_t<FlatMap2>>,
  typename FlatMap3, typename FlatMap3Result = InnerType<std::invoke_result_t<FlatMap3>>,
  typename FlatMap4, typename FlatMap4Result = InnerType<std::invoke_result_t<FlatMap4>>,
  typename FlatMap5, typename FlatMap5Result = InnerType<std::invoke_result_t<FlatMap5>>,
  typename FlatMap6, typename FlatMap6Result = InnerType<std::invoke_result_t<FlatMap6>>,
  typename FlatMap7, typename FlatMap7Result = InnerType<std::invoke_result_t<FlatMap7>>,
  typename FlatMap8, typename FlatMap8Result = InnerType<std::invoke_result_t<FlatMap8>>,
  typename FlatMap9, typename FlatMap9Result = InnerType<std::invoke_result_t<FlatMap9>>,
  typename Map
> requires
  std::invocable<FlatMap1>
  && std::invocable<FlatMap2>
  && std::invocable<FlatMap3>
  && std::invocable<FlatMap4>
  && std::invocable<FlatMap5>
  && std::invocable<FlatMap6>
  && std::invocable<FlatMap7>
  && std::invocable<FlatMap8>
  && std::invocable<FlatMap9>
  && std::invocable<Map, A, FlatMap1Result, FlatMap2Result, FlatMap3Result, FlatMap4Result, FlatMap5Result, FlatMap6Result, FlatMap7Result, FlatMap8Result, FlatMap9Result>
auto ForComprehension(
  FA fa,
  FlatMap1&& flatMap1,
  FlatMap2&& flatMap2,
  FlatMap3&& flatMap3,
  FlatMap4&& flatMap4,
  FlatMap5&& flatMap5,
  FlatMap6&& flatMap6,
  FlatMap7&& flatMap7,
  FlatMap8&& flatMap8,
  FlatMap9&& flatMap9,
  Map&& map
) {
  if constexpr (SmartPointer<FA> || PointerType<FA>) {
    return
      fa->flatMap([=](A a) { return
      flatMap1()->flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2()->flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3()->flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4()->flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5()->flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6()->flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7()->flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8()->flatMap([=](FlatMap8Result flatMap8Result) { return
      flatMap9()->map([=](FlatMap9Result flatMap9Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result, flatMap9Result);
      }); }); }); }); }); }); }); }); }); });  
  } else {
    return
      fa.flatMap([=](A a) { return
      flatMap1().flatMap([=](FlatMap1Result flatMap1Result) { return
      flatMap2().flatMap([=](FlatMap2Result flatMap2Result) { return
      flatMap3().flatMap([=](FlatMap3Result flatMap3Result) { return
      flatMap4().flatMap([=](FlatMap4Result flatMap4Result) { return
      flatMap5().flatMap([=](FlatMap5Result flatMap5Result) { return
      flatMap6().flatMap([=](FlatMap6Result flatMap6Result) { return
      flatMap7().flatMap([=](FlatMap7Result flatMap7Result) { return
      flatMap8().flatMap([=](FlatMap8Result flatMap8Result) { return
      flatMap9().map([=](FlatMap9Result flatMap9Result) { return
      map(a, flatMap1Result, flatMap2Result, flatMap3Result, flatMap4Result, flatMap5Result, flatMap6Result, flatMap7Result, flatMap8Result, flatMap9Result);
      }); }); }); }); }); }); }); }); }); });  
  }
}
