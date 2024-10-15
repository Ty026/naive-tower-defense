#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include "ecs.h"
#include "scene.h"
#include "type_id.h"

class SceneManager {
  using SceneFactory = std::unordered_map<uint32_t, std::function<Ref<Scene>(void)>>;
  using Scenes = std::vector<std::pair<uint32_t, Ref<Scene>>>;

public:
  template <typename T> requires std::is_base_of_v<Scene, T>
  void RegisterScene();

  template <typename T> requires std::is_base_of_v<Scene, T>
  void SwitchTo() { SwitchTo(TypeId<T>().hash()); }

  void SwitchTo(uint32_t scene_id);
  void Update();

  void Render();

private:
  Ref<Scene> CreateScene(uint32_t id);

  SceneFactory factory_;
  Scenes       scenes_;
  Registry     registry_;
};

template <typename T> requires std::is_base_of_v<Scene, T>
void SceneManager::RegisterScene() {
  auto id = TypeId<T>().hash();
  factory_[id] = []() { return MakeRef<T>(); };
}
