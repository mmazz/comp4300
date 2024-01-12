#pragma once
#include "common.h"
#include "physics.h"
#include "scene.h"


class Scene_Play : public Scene
{
  struct PlayerConfig {
    float X, Y, CX, CY, SPEED, MAXHEIGHT, JUMP, GRAVITY;
    std::string WEAPON;
  };


    protected:

        std::shared_ptr<Entity> m_player;
        std::string             m_levelPath;
        PlayerConfig            m_playerConfig;
        bool                    m_drawTextures = true;
        bool                    m_drawCollision = false;
        bool                    m_drawGrid = false;
        const Vec2              m_gridSize = {64, 64};
        sf::Text                m_gridText;

        void init(const std::string &levelPath);

        void loadLevel(const std::string &filePath);
        void loadMap(const std::string &filePath);
        void spawnPlayer();
        void update();
        void onEnd();

        void spawnBullet(const std::shared_ptr<Entity> &entity);
        void spawnEnemy();

        Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

        void sMovement();
        void sLifespan();
        void sCollision();
        void sDoAction(const Action &action) ;
        void sAnimation();
        void sRender();

    public:
    Scene_Play(GameEngine *gameEngine, const std::string &levelPath);
};
