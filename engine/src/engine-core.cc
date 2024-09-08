#include "common.h"
#include "engine-core.h"
#include <cassert>
#include "platform-window.h"
#include "renderer.h"
#include "texture.h"

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
  Camera2D c;
  c.set_viewport(window_->size());
  Renderer::Begin(c);
  uint8_t bitmapdata[] = {
      255, 0, 0, 255,
      0, 255, 0, 255,
      0, 0, 255, 255,
      255, 255, 0, 255};
  Ref<Texture> texture = MakeRef<Texture>(2, 2, 4, bitmapdata, false);
  Renderer::Draw({0, 0}, {100, 100}, Color::Yellow);
  Transform t;
  t.Translate({100, 100});
  t[0].x = 50;
  t[1].y = 50;
  Renderer::Draw(texture, {2, 2}, t, {0, 0}, {1, 1}, {0, 0});
  Renderer::End();
}
