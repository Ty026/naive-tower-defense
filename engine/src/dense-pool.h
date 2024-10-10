#pragma once

#include "common.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>
class ComponentPool {
public:
  ComponentPool(int element_size, int capacity) :
      element_size_(element_size), capacity_(capacity) {
    buffer_.reset(new std::byte[element_size_ * capacity_]);
    index_to_entity_.reserve(capacity_);
    entity_to_index_.reserve(capacity_);
  }

  template <typename T, typename... Args>
  T &Assign(uint32_t entity_id, Args &&...args) {
    if (Has(entity_id)) {
      T *component = new (buffer_.get() + entity_to_index_[entity_id] * element_size_) T(std::forward<Args>(args)...);
      return *component;
    }

    if (entity_id >= entity_to_index_.size()) {
      entity_to_index_.resize(entity_id + 1);
    }

    if (num_allocated_ >= capacity_) {
      capacity_ *= 2;
      auto new_buffer = new std::byte[element_size_ * capacity_];
      std::copy(buffer_.get(), buffer_.get() + num_allocated_ * element_size_, new_buffer);
      buffer_.reset(new_buffer);
      index_to_entity_.reserve(capacity_);
      entity_to_index_.reserve(capacity_);
    }

    entity_to_index_[entity_id] = index_to_entity_.size();
    index_to_entity_.emplace_back(entity_id);

    T *component = new (buffer_.get() + num_allocated_ * element_size_) T(std::forward<Args>(args)...);
    num_allocated_++;
    return *component;
  }

  template <typename T>
  T &Get(uint32_t entity_id) {
    assert(Has(entity_id));
    auto ptr = buffer_.get() + entity_to_index_[entity_id] * element_size_;
    return *reinterpret_cast<T *>(ptr);
  }

  void Release(uint32_t entity_id_to_remove) {
    assert(Has(entity_id_to_remove));

    auto last_index = index_to_entity_.size() - 1;
    entity_to_index_[index_to_entity_[last_index]] = entity_to_index_[entity_id_to_remove];
    index_to_entity_[entity_to_index_[entity_id_to_remove]] = index_to_entity_[last_index];

    if (entity_to_index_[entity_id_to_remove] != last_index) {
      auto src = buffer_.get() + last_index * element_size_;
      auto dst = buffer_.get() + entity_to_index_[entity_id_to_remove] * element_size_;
      std::copy(src, src + element_size_, dst);
    }

    index_to_entity_.pop_back();
    num_allocated_--;
  }

  bool Has(uint32_t entity_id) const {
    return (entity_id < entity_to_index_.size() &&
            entity_to_index_[entity_id] < index_to_entity_.size() &&
            index_to_entity_[entity_to_index_[entity_id]] == entity_id);
  }

  const std::vector<uint32_t> &entities() const { return index_to_entity_; }

  int size() const { return num_allocated_; }

private:
  int                   element_size_;
  int                   capacity_;
  Scope<std::byte[]>    buffer_;
  std::vector<uint32_t> entity_to_index_;
  std::vector<uint32_t> index_to_entity_;
  int                   num_allocated_{0};
};
