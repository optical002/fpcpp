#ifndef FPCPP_CORE_FUNCTIONAL_REF_CMP
#define FPCPP_CORE_FUNCTIONAL_REF_CMP

template<typename A>
bool refCmp(const A& left, const A& right) {
  return &left == &right;
}

#endif //FPCPP_CORE_FUNCTIONAL_REF_CMP
