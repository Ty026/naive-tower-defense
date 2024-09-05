#pragma once

#include "vec2.h"
struct WindowResizeEvent {
  vec2 size;
  real dpi_scale;
};

struct Event {
  enum class Type {
    WindowClose,
    WindowResize,
  };
  Type type;
  union {
    WindowResizeEvent window;
  };
};
