#include "entity.h"

Entity::Entity(const size_t &id, const std::string &entityTag)
    : m_tag(entityTag), m_id(id) {
}

bool Entity::isActive() const {
  return m_active;
}

void Entity::destroy() {
  m_active = false;
}

size_t Entity::id() const {
  return m_id;
}

const std::string & Entity::tag() const {
  return m_tag;
}
