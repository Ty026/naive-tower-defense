#include "scene_manager.h"
#include "log.h"

void SceneManager::SwitchTo(uint32_t id) {
  for (auto it = scenes_.begin(); it != scenes_.end(); ++it) {
    if (it->first == id) {
      scenes_.back().second->OnDeactivate();
      auto tmp = it->second;
      scenes_.erase(it);
      scenes_.emplace_back(id, tmp);
      tmp->OnActivate();
      return;
    }
  }

  auto new_scene = CreateScene(id);
  if (new_scene == nullptr) {
    Log::Error("Failed to create scene with id {},\nDo you forget to register the scene? ", id);
  }

  if (!scenes_.empty()) {
    scenes_.back().second->OnDeactivate();
  }
  scenes_.emplace_back(id, new_scene);
  new_scene->OnActivate();
}

Ref<Scene> SceneManager::CreateScene(uint32_t scene_id) {
  auto it = factory_.find(scene_id);
  if (it != factory_.end()) {
    auto scene = it->second();
    scene->set_registry(&registry_);
    scene->OnCreate();
    return scene;
  }
  return nullptr;
}

void SceneManager::Update() {
  auto it = scenes_.end();
  while (it != scenes_.begin()) {
    if (it != scenes_.end() && !it->second->transparent()) break;
    --it;
  }
  auto end = scenes_.end();
  for (; it != end; ++it) {
    it->second->OnUpdate();
  }
}

void SceneManager::Render() {
  auto it = scenes_.end();
  while (it != scenes_.begin()) {
    if (it != scenes_.end() && !it->second->transparent()) break;
    --it;
  }
  auto end = scenes_.end();
  for (; it != end; ++it) {
    it->second->OnRender();
  }
}
