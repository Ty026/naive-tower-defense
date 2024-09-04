#include "common.h"
#include "engine-core.h"
#include <cassert>
#include "platform-window.h"

Engine *Engine::instance_{nullptr};

Engine *Engine::Initialize(std::string_view title, int width, int height) {
  assert(instance_ == nullptr);
  instance_ = new Engine{title, width, height};
  return instance_;
}

Engine::Engine(std::string_view title, int width, int height) {
  window_ = MakeScope<Window>(title, width, height);
  window_->set_event_callback([this](const Event &e) { OnEvent(e); });
}

void Engine::OnEvent(const Event &e) {
  switch (e.type) {
  case Event::Type::WindowClose:
    is_running_ = false;
    break;
  default:
    // TODO
    break;
  }
}

void Engine::RunLoop() {
  while (is_running_) {
    window_->PollEvents();
    Tick();
    window_->Update();
  }
}

void Engine::Tick() {
}
