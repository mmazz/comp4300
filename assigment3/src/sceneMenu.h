#pragma once

#include "common.h"
#include "scenePlay.h"
#include <deque>
#include <map>
#include <memory>

#include "entityManager.h"

class Scene_Menu : public Scene
{

    protected:
        std::string              m_title;
        std::vector<std::string> m_menuStrings;
        std::vector<std::string> m_levelPaths;
        sf::Text                 m_menuText;
        size_t                   m_selectedMenuIndex = 0;

        void init();
        void update();
        void onEnd();
        void sDoAction(const Action &action);

    public:
        Scene_Menu(GameEngine *game);
        void sUserInput();
        void sRender();
};
