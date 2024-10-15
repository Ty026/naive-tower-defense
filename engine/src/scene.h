#pragma once
#include "ecs.h"
#include "event.h"
#include "system.h"

class Scene {
public:
  Scene() = default;
  virtual ~Scene() = default;
  virtual void OnCreate() {};
  virtual void OnActivate() {};
  virtual void OnDeactivate() {}
  virtual void OnDestroy() {}
  virtual void OnEvent(const Event &event) {}

  void OnUpdate();
  void OnRender();

  bool            transparent() { return transparent_; }
  const Registry &registry() const { return *registry_; }

  void set_registry(Registry *registry) { registry_ = registry; }
  void set_transparent(bool transparent) { transparent_ = transparent; }

  template <typename T> requires std::is_base_of<System, T>::value
  void AddSystem(bool is_render_system = false);

protected:
  Registry                *registry_{nullptr};
  bool                     transparent_{false};
  std::vector<Ref<System>> systems_;
};

template <typename T> requires std::is_base_of<System, T>::value
void Scene::AddSystem(bool is_render_system) {
  auto system = MakeRef<T>();
  system->set_type(is_render_system ? System::SystemType::Render : System::SystemType::Logic);
  systems_.push_back(system);
  system->set_registry(registry_);
  system->OnCreate();
}
