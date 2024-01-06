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
    LoadConfig(path);
    m_window.create(sf::VideoMode(m_width, m_height), "Assigment 2");
    m_window.setFramerateLimit(m_frameRate);

    spawnPlayer();
}

void Game::LoadConfig(const std::string & path)
{
    const std::string& filepath = "config.txt";
    std::ifstream stream(filepath);
    std::string line, word;
    fontData font;
    while (getline(stream, line))
    {
        std::stringstream ss(line);
        ss >> word;

        if(word == "Window")
            ss >> m_width >> m_height >> m_frameRate >> m_a;
        else if ( word == "Font")
            ss >> font.file >> font.size >> font.r >> font.g >> font.b;

        else if ( word == "Player")
        {
            ss >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S  >>
                  m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >>
                  m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >>
                  m_playerConfig.OT >> m_playerConfig.V;
        }
        else if (word == "Enemy" )
        {
            ss >> m_enemyConfig.SR   >> m_enemyConfig.CR   >>
                  m_enemyConfig.SMIN >> m_enemyConfig.SMAX >>
                  m_enemyConfig.OR   >> m_enemyConfig.OG   >> m_enemyConfig.OB   >>
                  m_enemyConfig.OT   >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >>
                  m_enemyConfig.L    >> m_enemyConfig.SI;
        }
        else if (word == "Bullet")
        {
            ss >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S  >>
                  m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >>
                  m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >>
                  m_bulletConfig.OT >> m_bulletConfig.V  >> m_bulletConfig.L;
        }
    }
    if (!m_font.loadFromFile(font.file))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }
}

void Game::run()
{

    while (m_running)
    {
        m_entities.update();

        if(!m_paused)
        {
            sEnemySpawner();
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
    float  shape = rand() % 1 + 16-8;
    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(1.0f, 1.0f), 0.0f);
    entity->cShape= std::make_shared<CShape>(32.0f, shape, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
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
            case sf::Keyboard::P: m_paused = !m_paused;
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
    float posX = m_player->cTransform->pos.x + m_player->cTransform->velocity.x;
    float posY = m_player->cTransform->pos.y + m_player->cTransform->velocity.y;
    float radius = m_player->cShape->circle.getRadius();
    std::cout << posX << " " << m_window.getSize().x << " " << radius<< std::endl;
    if(posX>0.0f and posX<m_window.getSize().x - radius)
        m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
    if(posY>0.0f and posY<m_window.getSize().y - radius)
        m_player->cTransform->pos.y += m_player->cTransform->velocity.x;
}

void Game::sLifespan()
{
}


void Game::sCollision()
{
    for(auto b: m_entities.getEntities("bullet"))
    {
        for (auto e: m_entities.getEntities("enemy"))
        {
         //   if( Physics::CheckCollision(b, e))
        //    {
                b->destroy();
                e->destroy();
         //   }
        }
    }
}

void Game::sEnemySpawner()
{
    if(m_lastEnemySpawnTime > 80)
    {
        spawnEnemy();
        m_lastEnemySpawnTime = 0;
    }
    else
        m_lastEnemySpawnTime++;
}

