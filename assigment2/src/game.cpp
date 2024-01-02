#include "game.h"

#include <SFML/Window/Keyboard.hpp>
#include <any>
#include <iostream>
#include <fstream>
#include <memory>

Game::Game(const std::string & config)
{
    init(config);
}

void Game::init(const std::string & path)
{
    //std::ifstream fin(path);
    m_window.create(sf::VideoMode(1280, 720), "Assigment 2");
    m_window.setFramerateLimit(60);

    spawnPlayer();
}

void Game::run()
{

    while (m_running)
    {
        m_entities.update();

        if(!m_paused)
        {
         //   sEnemySpawner();
            sMovement();
         //   sCollision();
        }
        sUserInput();
        sRender();

        m_currentFrame++;
    }
}

void Game::setPaused(bool paused)
{
}

void Game::spawnPlayer()
{
    auto entity = m_entities.addEntity("player");

    float mx = m_window.getSize().x / 2.0f;
    float my = m_window.getSize().y / 2.0f;
    entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);

    entity->cShape= std::make_shared<CShape>(32.0f, 8 ,sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
    //entity->cShape= std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
    entity->cInput = std::make_shared<CInput>();

    m_player = entity;
}
void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity("enemy");
    float ex = rand() % m_window.getSize().x;
    float ey = rand() % m_window.getSize().y;
    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1.0f, 1.0f), 0.0f);

    entity->cShape= std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);


    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::sRender()
{
    m_window.clear();

    for(auto e: m_entities.getEntities())
    {
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        e->cTransform->angle += 1.0f;
        e->cShape->circle.setRotation(e->cTransform->angle);

        m_window.draw(e->cShape->circle);
    }
    m_window.display();
}

void Game::sUserInput()
{

    sf::Event event;
    while(m_window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        if(event.type == sf::Event::KeyPressed)
        {
            switch(event.key.code)
            {
            case sf::Keyboard::Q: m_running = false;
                                  break;
            case sf::Keyboard::W: m_player->cInput->up = true;
                                  break;
            case sf::Keyboard::S: m_player->cInput->down = true;
                                  break;
            case sf::Keyboard::A: m_player->cInput->left = true;
                                  break;
            case sf::Keyboard::D: m_player->cInput->right = true;
                                  break;
            default: break;
            }
        }
        if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
            case sf::Keyboard::W: m_player->cInput->up   = false;
                                  break;
            case sf::Keyboard::S: m_player->cInput->down = false;
                                  break;
            case sf::Keyboard::A: m_player->cInput->left = false;
                                  break;
            case sf::Keyboard::D: m_player->cInput->right= false;
                                  break;
            default: break;
            }
        }
    }
}

void  Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
}

void  Game::spawnBullet(std::shared_ptr<Entity> e, const Vec2 & target)
{
}
void  Game::spawnSpecialWeapon(std::shared_ptr<Entity> e)
{
}

void Game::sMovement()
{
    m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
    m_player->cTransform->pos.y += m_player->cTransform->velocity.x;
}

void Game::sLifespan()
{
}


void Game::sCollision()
{
}

void Game::sEnemySpawner()
{
}

