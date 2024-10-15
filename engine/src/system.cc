#include "system.h"
#include <limits>
View System::GetEntities() {

  if (required_components_ids_.empty()) {
    return View(registry_, nullptr, 0, signature_);
  }
  uint32_t smallest_component_id = 0;
  num_entities_ = std::numeric_limits<uint32_t>::max();
  for (auto &id : required_components_ids_) {
    auto &pool = registry_->store().GetPool(id);
    if (pool.size() < num_entities_) {
      smallest_component_id = id;
      num_entities_ = pool.size();
    }
  }
  if (entities_.capacity() < num_entities_) {
    entities_.reserve(num_entities_);
  }
  auto pool_entites = registry_->store().GetPool(smallest_component_id).entities();
  entities_.assign(pool_entites.begin(), pool_entites.begin() + num_entities_);

  return View(registry_, entities_.data(), num_entities_, signature_);
}
