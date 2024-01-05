#pragma once

#include "components.h"
#include <memory>
#include <string>

enum class entityTags
{
	none,
	player,
	enemy,
	smallEnemy,
	bullet
};

class Entity
{
    // Por que hago que el contructor de Entity sea privado, y asi solo el
    // EntityManager es el que puede crear entidades
    friend class EntityManager;

    public:
        bool isActive() const;
        const std::string& tag() const;
        const size_t id() const;
        void destroy();

        std::shared_ptr<CTransform> cTransform;
        std::shared_ptr<CShape> cShape;
        std::shared_ptr<CCollision> cCollision;
        std::shared_ptr<CInput> cInput;
        std::shared_ptr<CScore> cScore;
        std::shared_ptr<CLifespan> cLifespan;

    private:
        std::string m_tag = "default";
        bool m_active = true;
        size_t m_id = 0;

        // al ser privado no puedo usar make_shared<Entity>
        Entity(const size_t id, const std::string& tag);
};
