#pragma once

#include <string_view>
#include "common.h"
#include "event.h"
#include "platform-window.h"
#include "scene_manager.h"

class Engine {
public:
  static Engine *Initialize(std::string_view title, int width, int height);
  static Engine *instance() { return instance_; }
  void           RunLoop();

  template <typename T>
  void RegisterScene() { scenes_.RegisterScene<T>(); }

  template <typename T>
  void SwitchTo() { scenes_.SwitchTo<T>(); }

private:
  static Engine *instance_;

  Engine(std::string_view title, int width, int height);
  void          OnEvent(const Event &e);
  void          Tick();
  Scope<Window> window_;
  bool          is_running_{true};

  SceneManager scenes_;
};
