#pragma once

#include "entity.h"
#include "entityManager.h"
#include "../include/imgui.h"
#include "../include/imgui-SFML.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; } ;
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L ; float S; };
struct fontData{
    std::string file;
    int size;
    float r, g, b;
};
class Game
{
    public:
        Game(const std::string& configFile);
        void run();

    private:
        sf::RenderWindow m_window;
        int m_width;
        int m_height;
        int m_frameRate;
        int m_a;
        EntityManager    m_entities;
        sf::Font         m_font;
        sf::Text         m_text;
        PlayerConfig     m_playerConfig;
        EnemyConfig      m_enemyConfig;
        BulletConfig     m_bulletConfig;
        sf::Clock        m_deltaClock;
        int              m_score = 0;
        int				 m_currentFrame = 0 ;
        int				 m_lastEnemySpawnTime = 0;
        bool             m_paused;
        bool             m_running = true;

        std::shared_ptr<Entity> m_player;

        void init(const std::string& config);			//initialize the gamestate with a config file path
        void setPaused(bool paused);					//Pause the game

        void LoadConfig(const std::string & path);
        void sMovement();								//Systems: entity movement
        void sUserInput();								//Systems: User Input
        void sLifespan();								//Systems: lifespan of the entities
        void sRender();									//Systems: Render
        void sEnemySpawner();							//Systems: Spawns enemies
        void sCollision();								//Systems: Collision detection

        void spawnPlayer();
        void spawnEnemy();
        void spawnSmallEnemies(std::shared_ptr<Entity> entity);
        void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
        void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
};

