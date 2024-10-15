#include "common.h"
#include "engine-core.h"
#include <cassert>
#include "platform-window.h"
#include "renderer.h"

Engine *Engine::instance_{nullptr};

Engine *Engine::Initialize(std::string_view title, int width, int height) {
  assert(instance_ == nullptr);
  instance_ = new Engine{title, width, height};
  return instance_;
}

Engine::Engine(std::string_view title, int width, int height) {
  window_ = MakeScope<Window>(title, width, height);
  window_->set_event_callback([this](const Event &e) { OnEvent(e); });
  Renderer::Initialize();
}

void Engine::OnEvent(const Event &e) {
  switch (e.type) {
  case Event::Type::WindowClose:
    is_running_ = false;
    break;
  case Event::Type::WindowResize: {
    Renderer::set_viewport(window_->framebuffer_size());
    break;
  }
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

  Renderer::Release();
}

void Engine::Tick() {
  Renderer::Clear();
  scenes_.Update();
  scenes_.Render();
}
