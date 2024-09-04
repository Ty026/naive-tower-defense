#pragma once

#include "event.h"
#include <functional>
#include <string_view>

class Window {
  using EventCallback = std::function<void(const Event &)>;

public:
  Window(const std::string_view title, int w, int h);
  void PollEvents();
  void Update();

  void set_event_callback(EventCallback handler) { event_handler_ = handler; };

private:
  EventCallback event_handler_;
  void         *raw_window_{nullptr};
};
