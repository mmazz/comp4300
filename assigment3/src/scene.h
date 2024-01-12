#pragma once

#include "action.h"
#include "entityManager.h"
#include <memory>


class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{

    protected:

        GameEngine *  m_game = nullptr;
        EntityManager m_entityManager;
        ActionMap     m_actionMap;
        bool          m_paused = false;
        bool          m_hasEnded = false;
        size_t        m_currentFrame = 0;

        virtual void onEnd() = 0;
        void setPaused(bool paused);

    public:
        Scene();
        Scene(GameEngine *game_engine);

        void simulate(const size_t frames);
        void registerAction(int inputKey, const std::string &actionName);

        size_t width();
        size_t height();
        size_t currentFrame() const;
        virtual void sDoAction(const Action &action) = 0;
        virtual void update() = 0;
        bool hasEnded() const;
        const ActionMap &getActionMap() const;
        void drawLine(const Vec2& p1, const Vec2& p2);
};
