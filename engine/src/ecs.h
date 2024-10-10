#pragma once

#include "dense-pool.h"
#include "log.h"
#include <bitset>
#include <concepts>
#include <cstdint>
#include <deque>
#include <vector>

using Entity = uint32_t;

constexpr const uint32_t kMaxComponents = 32;

using Signature = std::bitset<kMaxComponents>;

namespace internal {
static uint32_t NextComponentId() {
  static uint32_t id{};
  return id++;
};
}; // namespace internal
//
template <typename T>
struct ComponentId {
  static uint32_t value() {
    static const uint32_t id = internal::NextComponentId();
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

  template <typename T>
  ComponentPool &GetPool() {
    uint32_t component_id = ComponentId<T>();
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

class Registry {
public:
  template <typename T, typename... Args> requires std::constructible_from<T, Args...>
  void AddComponent(uint32_t entity, Args &&...args) {
    const uint32_t component_id = ComponentId<T>();
    component_signatures_[entity].set(component_id);
    store_.Assign<T>(entity, std::forward<Args>(args)...);
  }

  template <typename T>
  T &GetComponent(uint32_t entity) {
    return store_.GetPool<T>().template Get<T>(entity);
  }

  template <typename T>
  void RemoveComponent(uint32_t entity) {
    uint32_t component_id = ComponentId<T>();
    if (component_signatures_[entity].test(component_id)) {
      store_.GetPool<T>().template Release(entity);
    }
    component_signatures_[entity].set(component_id, false);
  }

  template <typename T>
  bool HasComponent(uint32_t entity) const;

  uint32_t Create() {
    int id;
    if (free_ids_.empty()) {
      id = num_entities_++;
      if (id >= component_signatures_.size()) {
        component_signatures_.resize(id + 1);
      }
    } else {
      id = free_ids_.front();
      free_ids_.pop_front();
    }
    entities_to_add_.push_back(id);
    return id;
  }

  void Update() {
    entities_to_add_.clear();
    for (auto id_to_remove : entities_to_remove_) {
      free_ids_.push_back(id_to_remove);
      component_signatures_[id_to_remove].reset();
    }
    entities_to_remove_.clear();
  }

  uint32_t num_entities() const { return num_entities_; }

  ComponentStore store_;

private:
  int                    num_entities_{0};
  std::deque<int>        free_ids_;
  std::vector<Signature> component_signatures_;
  std::vector<int>       entities_to_add_;
  std::vector<int>       entities_to_remove_;
};

// TODO
template <typename T, typename... RestComponents>
class View {
  class Iterator {
    //   bool Valid() const {
    //     return false;
    //   }
    //
  public:
    Iterator(uint32_t pos) :
        pos_(pos) {}
    Iterator &operator++() {
      if (pos_ != 0) {
        --pos_;
      }
      return *this;
    }
    bool     operator!=(const Iterator &other) const { return pos_ != other.pos_; }
    uint32_t operator*() const {
      return pos_;
    }

  private:
    uint32_t pos_;
  };

public:
  View(ComponentStore &store, Registry &registry) :
      registry_(registry),
      store_(store) {
    auto &pool = store.GetPool<T>();
    size_ = pool.size();
    signature_.set(ComponentId<T>());
    entities_ = pool.entities().data();
    int accumulator[] = {0, (GetSmallestComponentSet<RestComponents>(), 0)...};
    (void)accumulator;
  }

  template <typename Component>
  void GetSmallestComponentSet() {
    signature_.set(ComponentId<Component>());
    auto &pool = store_.GetPool<Component>();
    auto  size = pool.size();
    if (size < size_) {
      entities_ = pool.entities().data();
      size_ = size;
    }
  }

  Iterator begin() const {
    return Iterator{size_};
  }

  Iterator end() const {
    return Iterator{0};
  }

private:
  uint32_t        size_{0};
  const uint32_t *entities_{nullptr};
  ComponentStore &store_;
  Registry       &registry_;
  Signature       signature_;
};
