#pragma once

#include <cstddef>
#include <cstdint>
class StringId {
  struct Fnv1a {
    static constexpr uint32_t kOffsetBasis = 0x811C9DC5;
    static constexpr uint32_t kPrime = 0x01000193;
  };

public:
  static constexpr uint32_t value(const char *str, const size_t length) {
    return StringId(str, length);
  }

  template <size_t N>
  static constexpr uint32_t value(const char (&str)[N]) {
    return StringId(str);
  }

  static constexpr uint32_t value(const char *str) {
    return StringId(str);
  }

  constexpr StringId() :
      StringId(nullptr, 0) {}

  constexpr StringId(const char *str, const size_t length) {
    str_repr_ = str;
    length_ = length;
    hash_ = Fnv1a::kOffsetBasis;
    for (size_t i = 0; i < length; ++i) {
      hash_ = (hash_ ^ static_cast<uint32_t>(str[i])) * Fnv1a::kPrime;
    }
  }

  template <size_t N>
  constexpr StringId(const char (&str)[N]) {
    str_repr_ = str;
    hash_ = Fnv1a::kOffsetBasis;
    for (; str[length_]; ++length_) {
      hash_ = (hash_ ^ static_cast<uint32_t>(str[length_])) * Fnv1a::kPrime;
    }
  }

  explicit constexpr StringId(const char *str) {
    str_repr_ = str;
    hash_ = Fnv1a::kOffsetBasis;
    for (; str[length_]; ++length_) {
      hash_ = (hash_ ^ static_cast<uint32_t>(str[length_])) * Fnv1a::kPrime;
    }
  }

  constexpr size_t      size() const { return length_; }
  constexpr const char *data() const { return str_repr_; }
  constexpr uint32_t    value() const { return hash_; }
  constexpr             operator uint32_t() const { return hash_; }

private:
  const char *str_repr_{nullptr};
  size_t      length_{0};
  uint32_t    hash_{0};
};
