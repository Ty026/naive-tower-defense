#pragma once

struct Event {
  enum class Type {
    WindowClose,
  };
  Type type;
};
