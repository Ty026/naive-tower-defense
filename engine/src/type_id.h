#pragma once

#include <cstdint>
#include <string_view>
#include <utility>
#include <string_id.h>

namespace {
static uint32_t NextId() {
  static uint32_t id{};
  return id++;
};

template <typename T>
constexpr auto StrippedTypeName() {
  std::string_view fn_name{__PRETTY_FUNCTION__};
  auto             first = fn_name.find_first_not_of(' ', fn_name.find_first_of('=') + 1);
  auto             name = fn_name.substr(first, fn_name.find_last_of(']') - first);
  return name;
}
} // namespace

template <typename T>
struct TypeIndex {
  static uint32_t value() {
    static const uint32_t id = NextId();
    return id;
  }
  constexpr operator uint32_t() const { return value(); }
};

template <typename T>
struct TypeHash {
  static constexpr uint32_t value() {
    constexpr auto stripped_name = StrippedTypeName<T>();
    return StringId(stripped_name.data(), stripped_name.size());
  }
  constexpr operator uint32_t() const { return value(); }
};

template <typename T>
struct TypeName {
  static constexpr std::string_view value() {
    constexpr auto name = StrippedTypeName<T>();
    return name;
  }

  constexpr operator std::string_view() const {
    return value();
  }
};

class TypeInfo {
public:
  template <typename T>
  constexpr TypeInfo(std::in_place_type_t<T>) :
      sequence_(TypeIndex<std::remove_cv_t<std::remove_reference_t<T>>>()),
      id_(TypeHash<std::remove_cv_t<std::remove_reference_t<T>>>()),
      alias_(TypeName<std::remove_cv_t<std::remove_reference_t<T>>>()) {}

  constexpr uint32_t index() const {
    return sequence_;
  }

  constexpr uint32_t hash() const {
    return id_;
  }

  constexpr std::string_view name() const {
    return alias_;
  }

  constexpr bool operator==(const TypeInfo &rhs) const {
    return hash() == rhs.hash();
  }

  constexpr bool operator!=(const TypeInfo &rhs) const {
    return !(id_ == rhs.id_);
  }

private:
  uint32_t         sequence_;
  uint32_t         id_;
  std::string_view alias_;
};

template <typename T>
const TypeInfo &TypeId() {
  if constexpr (std::is_same_v<T, std::remove_cv_t<std::remove_reference_t<T>>>) {
    static TypeInfo instance{std::in_place_type<T>};
    return instance;
  } else {
    return TypeId<std::remove_cv_t<std::remove_reference_t<T>>>();
  }
}

template <typename Type>
const TypeInfo &TypeId(Type &&) {
  return TypeId<std::remove_cv_t<std::remove_reference_t<Type>>>();
}
