#pragma once

#include <memory>

template <typename T, typename Deleter = std::default_delete<T>>
using Scope = std::unique_ptr<T, Deleter>;

template <typename T, typename... Args>
requires std::constructible_from<T, Args...>
constexpr Scope<T> MakeScope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}
