#pragma once

#include "common.h"
#include "event.h"
#include "vec2.h"
#include <functional>
#include <string_view>

class Window {
  using EventCallback = std::function<void(const Event &)>;

public:
  Window(const std::string_view title, int w, int h);
  void PollEvents();
  void Update();

  const vec2 &size() const { return size_; }
  const vec2  framebuffer_size() const { return size_ * dpi_scale_; }
  void        set_event_callback(EventCallback handler) { event_handler_ = handler; };

private:
  EventCallback event_handler_;
  void         *raw_window_{nullptr};
  real          dpi_scale_{1};
  vec2          size_{800, 600};
};
