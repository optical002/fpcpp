#ifndef FPCPP_CORE_DATA_VARIANT_H
#define FPCPP_CORE_DATA_VARIANT_H

#include <cstddef>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <array>
#include <cstring>
#include <tuple>
#include <sstream>
#include <string>
#include <core/data/Option.h>
#include <core/typeclasses/ToString.h>
#include <core/typeclasses/Eq.h>

template<typename A>
class Option;

/**
 * @brief A flexible type-safe union that can hold one of several specified types.
 *
 * The 'Variant' does not restrict to having unique types like std::variant.
 * It's memory size is 'std::size_t' + the largest size from all 'Types...'.
 * Does not allocate on heap.
 *
 * @note Eq compares byte array instead of using specific type Eq comparators.
 */
template<typename... Types>
struct Variant {
public:
  template<std::size_t Index, typename Data>
  requires
    (Index < sizeof...(Types))
    && std::same_as<Data, std::tuple_element_t<Index, std::tuple<Types...>>>
  static Variant create(Data data) {
    Variant variant(Index);
    std::memcpy(variant._storage.data(), reinterpret_cast<std::byte*>(&data), sizeof(Data));
    return variant;
  }

  template<std::size_t Index, typename Data = std::tuple_element_t<Index, std::tuple<Types...>>>
  requires
    (Index < sizeof...(Types))
    && std::same_as<Data, std::tuple_element_t<Index, std::tuple<Types...>>>
  Option<Data> get() const {
    return _index == Index
      ? Option<Data>::some(*reinterpret_cast<Data*>(const_cast<std::byte*>(_storage.data())))
      : Option<Data>::none();;
  }

  template<std::size_t Index, typename Data = std::tuple_element_t<Index, std::tuple<Types...>>>
  requires
    (Index < sizeof...(Types))
    && std::same_as<Data, std::tuple_element_t<Index, std::tuple<Types...>>>
  Data _unsafe_get() const {
    if (_index != Index) {
      throw std::bad_variant_access();
    }
    return *reinterpret_cast<Data*>(const_cast<std::byte*>(_storage.data()));
  }

  bool isValueAtIdx(const std::size_t idx) const { return _index == idx; }

  std::size_t index() const { return _index; }

private:
  explicit Variant(const std::size_t index) : _index(index) {}

  std::size_t _index;
  std::array<std::byte, (std::max)({sizeof(Types)...})> _storage = {};

  friend class Eq<Variant>;
  friend class ToString<Variant>;
};

template<HasEq... Types>
struct Eq<Variant<Types...>> {
  static bool equal(const Variant<Types...>& a, const Variant<Types...>& b) {
    return
      a.index() == b.index()
      && std::equal(a._storage.cbegin(), a._storage.cend(), b._storage.cbegin());
  }
};

template<HasToString... Types>
struct ToString<Variant<Types...>> {
  static std::string toStr(const Variant<Types...>& v) {
    std::ostringstream byteStrStream;
    for (const auto& byte : v._storage) {
      byteStrStream << std::hex << std::setw(2) << std::setfill('0')
          << static_cast<int>(std::to_integer<unsigned char>(byte));
    }
    return std::format("Variant(index={}, bytes=0x{})", v.index(), byteStrStream.str());
  }
};

#endif //FPCPP_CORE_DATA_VARIANT_H
