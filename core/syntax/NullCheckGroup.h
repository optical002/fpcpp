#ifndef FPCPP_CORE_SYNTAX_NULL_CHECK_GROUP_H
#define FPCPP_CORE_SYNTAX_NULL_CHECK_GROUP_H

#include <type_traits>
#include <memory>
#include <core/data/Functions.h>
#include <core/data/Option.h>

template<typename Ptr1> requires std::is_pointer_v<Ptr1>
Option<Ptr1> NullCheckGroup(Ptr1 ptr1) {
  if (ptr1 == nullptr) { return None; }
  return Some(ptr1);
}
template<
  typename Ptr1, typename Ptr2
> requires std::is_pointer_v<Ptr1> && std::is_pointer_v<Ptr2>
Option<std::tuple<Ptr1, Ptr2>> NullCheckGroup(Ptr1 ptr1, Ptr2 ptr2) {
  if (ptr1 == nullptr || ptr2 == nullptr) { return None; }
  return Some(Tpl(ptr1, ptr2));
}

template<
  typename Ptr1, typename Ptr2, typename Ptr3
> requires std::is_pointer_v<Ptr1> && std::is_pointer_v<Ptr2> && std::is_pointer_v<Ptr3>
Option<std::tuple<Ptr1, Ptr2, Ptr3>> NullCheckGroup(Ptr1 ptr1, Ptr2 ptr2, Ptr3 ptr3) {
  if (ptr1 == nullptr || ptr2 == nullptr || ptr3 == nullptr) { return None; }
  return Some(Tpl(ptr1, ptr2, ptr3));
}

template<
  typename Ptr1, typename Ptr2, typename Ptr3, typename Ptr4
> requires std::is_pointer_v<Ptr1> && std::is_pointer_v<Ptr2> && std::is_pointer_v<Ptr3> && std::is_pointer_v<Ptr4>
Option<std::tuple<Ptr1, Ptr2, Ptr3, Ptr4>> NullCheckGroup(Ptr1 ptr1, Ptr2 ptr2, Ptr3 ptr3, Ptr4 ptr4) {
  if (ptr1 == nullptr || ptr2 == nullptr || ptr3 == nullptr || ptr4 == nullptr) { return None; }
  return Some(Tpl(ptr1, ptr2, ptr3, ptr4));
}

#endif //FPCPP_CORE_SYNTAX_NULL_CHECK_GROUP_H
