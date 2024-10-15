#pragma once

#include "common.h"
#include "dense-pool.h"
#include <bitset>
#include <concepts>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <vector>

constexpr const uint32_t kMaxComponents = 32;

using Signature = std::bitset<kMaxComponents>;

namespace internal {
static uint32_t GetNextComponentTypeId() {
  static uint32_t id{};
  return id++;
};
}; // namespace internal

template <typename T>
struct ComponentTypeId {
  static uint32_t value() {
    static const uint32_t id = internal::GetNextComponentTypeId();
    return id;
  }
  constexpr operator uint32_t() const { return value(); }
};

class ComponentStore {
public:
  template <typename T, typename... Args>
  T &Assign(uint32_t entity_id, Args &&...args) {
    return GetPool<T>().template Assign<T>(entity_id, std::forward<Args>(args)...);
  }

  ComponentPool &GetPool(uint32_t component_id) {
    assert(component_id < pools_.size());
    return *(pools_[component_id].get());
  }

  template <typename T>
  ComponentPool &GetPool() {
    uint32_t component_id = ComponentTypeId<T>();
    if (component_id >= pools_.size()) {
      pools_.resize(component_id + 1);
    }
    if (pools_[component_id] == nullptr) {
      pools_[component_id] = MakeScope<ComponentPool>(sizeof(T), 2000);
    }
    return *(pools_[component_id].get());
  }

private:
  std::vector<Scope<ComponentPool>> pools_;
};

class Registry;

class Entity {
public:
  Entity(uint32_t id, Registry *registry) :
      id_(id),
      registry_(registry) {}
  uint32_t id() const { return id_; }

  template <typename T, typename... Args>
  requires std::is_constructible<T, Args...>::value
  T &AddComponent(Args &&...args);

  template <typename T>
  void RemoveComponent();

  template <typename T>
  T &GetComponent() const;

  template <typename T>
  bool HasComponent() const;

private:
  uint32_t  id_{0};
  Registry *registry_;
};

class View;

class Registry {
public:
  template <typename T, typename... Args> requires std::constructible_from<T, Args...>
  T &AddComponent(uint32_t entity, Args &&...args) {
    const uint32_t component_id = ComponentTypeId<T>();
    component_signatures_[entity].set(component_id);
    return store_.Assign<T>(entity, std::forward<Args>(args)...);
  }

  template <typename T>
  T &GetComponent(uint32_t entity) {
    return store_.GetPool<T>().template Get<T>(entity);
  }

  template <typename T>
  void RemoveComponent(uint32_t entity) {
    uint32_t component_id = ComponentTypeId<T>();
    if (component_signatures_[entity].test(component_id)) {
      store_.GetPool<T>().Release(entity);
    }
    component_signatures_[entity].set(component_id, false);
  }

  bool HasComponent(uint32_t entity, const Signature &signature) const {
    return (component_signatures_[entity] & signature) == signature;
  }

  template <typename T>
  bool HasComponent(uint32_t entity) const {
    return HasComponent(entity, ComponentTypeId<T>());
  }

  uint32_t Create() {
    int id;
    if (available_entity_ids_.empty()) {
      id = num_entities_++;
      if (id >= component_signatures_.size()) {
        component_signatures_.resize(id + 1);
      }
    } else {
      id = available_entity_ids_.front();
      available_entity_ids_.pop_front();
    }
    return id;
  }

  void Destroy(uint32_t entity) {
    RemoveAllComponents(entity);
    available_entity_ids_.push_back(entity);
  }

  void RemoveAllComponents(uint32_t entity) {
    auto &signature = component_signatures_[entity];
    for (uint32_t i = 0; i < signature.size(); ++i) {
      if (signature.test(i)) {
        store_.GetPool(i).Release(entity);
      }
    }
    signature.reset();
  }

  uint32_t        num_entities() const { return num_entities_; }
  ComponentStore &store() { return store_; }

private:
  int                    num_entities_{0};
  std::deque<int>        available_entity_ids_;
  std::vector<Signature> component_signatures_;
  ComponentStore         store_;
};

class View {
  class Iterator {
    bool IsValidEntity() const {
      return registry_->HasComponent(entities_[current_index_ - 1], match_signature_);
    }

  public:
    Iterator(const Registry *registry, const uint32_t *entities, uint32_t pos, Signature match_signature) :
        current_index_(pos),
        registry_(registry),
        match_signature_(match_signature),
        entities_(entities) {
      if (pos)
        while (!IsValidEntity() && --current_index_);
    }
    Iterator &operator++() {
      if (current_index_ != 0) {
        while (--current_index_ && !IsValidEntity());
      }
      return *this;
    }
    bool operator!=(const Iterator &other) const {
      return current_index_ != other.current_index_;
    }
    uint32_t operator*() const { return entities_[current_index_ - 1]; }

  private:
    uint32_t        current_index_;
    Signature       match_signature_;
    const Registry *registry_;
    const uint32_t *entities_;
  };

public:
  View(Registry *registry, const uint32_t *entities, uint32_t pos, Signature match_signature) :
      registry_(registry),
      entities_(entities),
      entity_count_(pos),
      signature_(match_signature) {}

  Iterator begin() const {
    return Iterator{registry_, entities_, entity_count_, signature_};
  }

  Iterator end() const {
    return Iterator{registry_, entities_, 0, signature_};
  }

private:
  uint32_t        entity_count_{0};
  const uint32_t *entities_{nullptr};
  Registry       *registry_;
  Signature       signature_;
};

// template <typename T, typename... Args> requires std::is_base_of<System, T>::value
// Ref<T> Registry::AddSystem() {
//   auto system = MakeRef<T>();
//   system->set_registry(this);
//   system->OnCreate();
//   systems_.push_back(system);
//   return system;
// }

template <typename T, typename... Args>
requires std::is_constructible<T, Args...>::value
T &Entity::AddComponent(Args &&...args) {
  return registry_->AddComponent<T>(id_, std::forward<Args>(args)...);
}

template <typename T>
void Entity::RemoveComponent() {
  registry_->RemoveComponent<T>(id_);
}

template <typename T>
T &Entity::GetComponent() const {
  return registry_->GetComponent<T>(id_);
}

template <typename T>
bool Entity::HasComponent() const {
  return registry_->HasComponent<T>(id_);
}
