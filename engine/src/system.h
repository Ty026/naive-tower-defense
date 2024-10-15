#pragma once
#include "ecs.h"
#include <cstdint>
#include <vector>
class System {
  using Entities = std::vector<uint32_t>;

public:
  enum class SystemType {
    Logic,
    Render
  };
  System() = default;
  virtual void OnCreate() {}
  virtual void OnUpdate() = 0;
  View         GetEntities();

  template <typename T, typename... RestComponents>
  void RequireComponents();

  SystemType type() const { return type_; }
  void       set_registry(Registry *registry) { registry_ = registry; }
  void       set_type(SystemType type) { type_ = type; }

protected:
  Registry  *registry_;
  SystemType type_{SystemType::Logic};

private:
  template <typename Component>
  void GenerateComponentTypeId();

  std::vector<uint32_t> required_components_ids_;
  Signature             signature_;
  Entities              entities_;
  uint32_t              num_entities_{0};
};

template <typename T, typename... RestComponents>
void System::RequireComponents() {
  required_components_ids_.clear();
  GenerateComponentTypeId<T>();
  (GenerateComponentTypeId<RestComponents>(), ...);
}

template <typename Component>
void System::GenerateComponentTypeId() {
  uint32_t id = ComponentTypeId<Component>();
  assert(std::find(required_components_ids_.begin(), required_components_ids_.end(), id) == required_components_ids_.end());
  required_components_ids_.push_back(id);
  (void)registry_->store().GetPool<Component>();
  signature_.set(id);
}
