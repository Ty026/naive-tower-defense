#include "scene.h"

void Scene::OnUpdate() {
  for (auto system : systems_) {
    if (system->type() == System::SystemType::Logic) system->OnUpdate();
  }
}

void Scene::OnRender() {
  for (auto system : systems_) {
    if (system->type() == System::SystemType::Render) system->OnUpdate();
  }
}
