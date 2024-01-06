#include "entityManager.h"
#include <memory>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{

    for (auto e: m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }

    m_entitiesToAdd.clear();
    removeDeadEntities(m_entities);

    for(auto& [tag, entityVec] : m_entityMap)
        removeDeadEntities(entityVec);
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    std::erase_if(vec, [](auto& entity) { return !entity->isActive(); });
   // std::vector<std::shared_ptr<Entity>> dead_entities;
   // for(auto e: vec)
   // {
   //     if(!e->isActive())
   //         dead_entities.push_back(e);
   // }
   // for(auto e: dead_entities)
   //     vec->erase(e);
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
    auto entity =std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
    return m_entityMap[tag];
}
