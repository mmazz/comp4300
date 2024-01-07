#include "game.h"
#include "components.h"

#include <any>
#include <complex>
#include <imgui-SFML.h>
#include <iostream>
#include <fstream>
#include <memory>

inline extern const double PI{ 3.14159265359 };

Game::Game(const std::string & config)
{
    init(config);
}

void Game::init(const std::string & path)
{
    LoadConfig(path);
    if(m_fullScreen)
        m_window.create(sf::VideoMode(m_width, m_height), "Assigment 2", sf::Style::Fullscreen);
    else
        m_window.create(sf::VideoMode(m_width, m_height), "Assigment 2", sf::Style::Titlebar);

    m_window.setFramerateLimit(m_frameRate);
	m_text.setPosition(0, 0);
	m_text.setString("Score: " + std::to_string(m_score));
    ImGui::SFML::Init(m_window, true);
    ImGui::GetStyle().ScaleAllSizes(1.0f);

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
            ss >> m_width >> m_height >> m_frameRate >> m_fullScreen;
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
                  m_enemyConfig.L    >> m_enemyConfig.SP;
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
    m_text.setFont(m_font);
    m_text.setCharacterSize(font.size);
    m_text.setFillColor(sf::Color(font.r, font.g, font.b));
}

void Game::run()
{
    while (m_running)
    {
        m_entities.update();

        ImGui::SFML::Update(m_window, m_deltaClock.restart());
        if(!m_paused)
        {
            if(m_spawn)
                sEnemySpawner();
            if (m_movement)
                sMovement();
            if(m_collision)
                sCollision();
            if(m_lifespan)
                sLifespan();
        }
        sGUI();
        sUserInput();
        if(m_render)
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

    entity->cShape= std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V,
            sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

    entity->cInput     = std::make_shared<CInput>();
    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
    entity->cScore     = std::make_shared<CScore>(1.0f);

    m_player = entity;
}

void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity("enemy");
    float ex = rand() % (m_width-2);
    float ey = rand() % (m_height-2);
    // Esta mal esto...
    float speed = rand() % (int)(1+ m_enemyConfig.SMAX - m_enemyConfig.SMIN);
    Vec2 norm{ex, ey};
    norm.normalize();
    Vec2 velocity{speed*norm.x, speed*norm.y};
    float r = rand() % 255;
    float g = rand() % 255;
    float b = rand() % 255;
    float  shape = rand() % (1 + m_enemyConfig.VMAX-m_enemyConfig.VMIN);
    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), velocity, 0.0f);
    entity->cShape= std::make_shared<CShape>(m_enemyConfig.SR, shape, sf::Color(r, g, b),
            sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
    entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
    entity->cCollision= std::make_shared<CCollision>(m_enemyConfig.CR);
    m_lastEnemySpawnTime = m_currentFrame;
}

void  Game::spawnBullet(std::shared_ptr<Entity> e, const Vec2 & target)
{
    auto entity = m_entities.addEntity("bullet");
    float speed = m_bulletConfig.S;
	Vec2 difference{ target.x - e->cTransform->pos.x, target.y - e->cTransform->pos.y };
	difference.normalize();
	Vec2 velocity{m_bulletConfig.S * difference.x, m_bulletConfig.S * difference.y};
    entity->cTransform = std::make_shared<CTransform>(Vec2(e->cTransform->pos.x, e->cTransform->pos.y), velocity, 0.0f);
    float  shape = m_bulletConfig.V;
    entity->cShape= std::make_shared<CShape>(m_bulletConfig.SR, shape, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
            sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
    entity->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
    entity->cCollision= std::make_shared<CCollision>(m_bulletConfig.CR);

}
void Game::sRender()
{
    m_window.clear();

    for(auto e: m_entities.getEntities())
        m_window.draw(e->cShape->circle);

    m_window.draw(m_text);
    ImGui::SFML::Render(m_window);
    m_window.display();
}

void Game::sMovement()
{
    float posX = m_player->cTransform->pos.x + m_playerConfig.S;
    float posY = m_player->cTransform->pos.y + m_playerConfig.S;
    float radius = m_playerConfig.CR;

    //TODO up  y left penetran un poco
    if(m_player->cInput->left)
    {
        if(posX > radius)
            m_player->cTransform->pos.x -=  m_playerConfig.S;
    }
    if(m_player->cInput->right)
    {
        if(posX < m_window.getSize().x - radius)
            m_player->cTransform->pos.x +=  m_playerConfig.S;
    }
    if(m_player->cInput->up)
    {
        if(posY > radius)
            m_player->cTransform->pos.y -=  m_playerConfig.S;
    }
    if(m_player->cInput->down)
    {
        if(posY < m_window.getSize().y - radius)
            m_player->cTransform->pos.y +=  m_playerConfig.S;
    }

    for(auto e: m_entities.getEntities())
    {
        e->cTransform->pos.x += e->cTransform->velocity.x;
        e->cTransform->pos.y += e->cTransform->velocity.y;
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        e->cTransform->angle += 1.0f;
        e->cShape->circle.setRotation(e->cTransform->angle);
    }

}



void Game::sCollision()
{
    // NO importa la eficienca aca. Loopeo todos los enemigos y veo si alguno choco contra el personaje
    // despues loopeo todas las balas y me fijo loopeando todos los enemigos si alguno toco

    for (auto enemy: m_entities.getEntities("enemy"))
    {
        int posX = enemy->cTransform->pos.x;
        int posY = enemy->cTransform->pos.y;
        int CR   = enemy->cCollision->radius;

        if(posX < CR or posX > m_window.getSize().x- CR)
        {
           enemy->cTransform->velocity.x *= -1;
           enemy->cTransform->pos.x += 2*enemy->cTransform->velocity.x;
        }

        if(posY < CR or posY > m_window.getSize().y - CR)
        {
           enemy->cTransform->velocity.y *= -1;
           enemy->cTransform->pos.y += 2*enemy->cTransform->velocity.y;
        }

        Vec2 diff{ enemy->cTransform->pos.x - m_player->cTransform->pos.x , enemy->cTransform->pos.y - m_player->cTransform->pos.y };

        double collisionRadiusSQ{ (m_player->cCollision->radius + enemy->cCollision->radius) * (m_player->cCollision->radius + enemy->cCollision->radius) };
        double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

        if (distSQ < collisionRadiusSQ)
        {
            m_score = 0;

            spawnSmallEnemies(enemy);
            enemy->destroy();
            m_player->destroy();
            spawnPlayer();

        }

    }
    for (auto bullet: m_entities.getEntities("bullet"))
    {
        for (auto enemy: m_entities.getEntities("enemy"))
        {
            Vec2 diff{ enemy->cTransform->pos.x - bullet->cTransform->pos.x , enemy->cTransform->pos.y - bullet->cTransform->pos.y };

            double collisionRadiusSQ{ (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy->cCollision->radius) };
            double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

            if (distSQ < collisionRadiusSQ)
            {
                //Updates the score
                std::cout << m_score << std::endl;
                m_score += m_player->cScore->score;
                m_text.setString("Score: " + std::to_string(m_score));

                spawnSmallEnemies(enemy);
                enemy->destroy();
                bullet->destroy();
            }
        }
    }
    for (auto e: m_entities.getEntities("enemy"))
    {
        for (auto enemy: m_entities.getEntities("enemy"))
        {
            Vec2 diff{ enemy->cTransform->pos.x - e->cTransform->pos.x , enemy->cTransform->pos.y - e->cTransform->pos.y };

            double collisionRadiusSQ{ (e->cCollision->radius + enemy->cCollision->radius) * (e->cCollision->radius + enemy->cCollision->radius) };
            double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

            if (distSQ < collisionRadiusSQ)
            {

            }
        }
    }
}

void Game::sUserInput()
{

    sf::Event event;
    while(m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(m_window, event);

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
        //    case sf::Keyboard::P: m_paused = !m_paused;
       //                           break;
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

        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(ImGui::GetIO().WantCaptureMouse) { continue; }

            if(event.mouseButton.button == sf::Mouse::Left)
            {
                const Vec2 mousePos{ static_cast<float>(sf::Mouse::getPosition(m_window).x), static_cast<float>(sf::Mouse::getPosition(m_window).y) };
                spawnBullet(m_player, mousePos);
            }

            if(event.mouseButton.button == sf::Mouse::Right)
                spawnSpecialWeapon(m_player);

        }
    }
}

void  Game::sGUI()
{
    ImGui::Begin("Geometry Wars");
    ImGui::Text("Geometry Wars");
    ImGui::Checkbox("Movement", &m_movement);
    ImGui::Checkbox("Lifespan", &m_lifespan);
    ImGui::Checkbox("Collision", &m_collision);
    ImGui::Checkbox("Spawning", &m_spawn);
    ImGui::SliderInt("Spawn rate", &m_enemyConfig.SP, 0, 300);
    ImGui::SliderInt("Enemy Life", &m_enemyConfig.L, 0, 1000);
    ImGui::Checkbox("Rendering", &m_render);
    ImGui::End();
}

void  Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    int degree = e->cShape->circle.getPointCount();
    std::cout << "degree " << degree << std::endl;
    float angleDelta = 360.0f/degree * PI / 180.0 ;
    std::cout << "angle " << angleDelta<< std::endl;
    float posX = e->cTransform->pos.x;
    float posY = e->cTransform->pos.y;
    float speed = 2.0f;
    Vec2 norm{posX, posY};
    norm.normalize();
    for(int i=0; i<degree; i++)
    {
        auto entity = m_entities.addEntity("smallEnemy");

        float Xchange = cos((i+1) * angleDelta);
        float Ychange = sin((i+1) * angleDelta);
        std::cout << Xchange << " " << Ychange << std::endl;
        std::cout << Xchange*speed*norm.x << " " << Ychange*speed*norm.y<< std::endl;
        std::cout << std::endl;
        Vec2 velocity{Xchange*speed*norm.x*e->cTransform->velocity.x, Ychange*speed*norm.y*e->cTransform->velocity.y};

        entity->cTransform = std::make_shared<CTransform>(Vec2(posX, posY), velocity, 0.0f);

        entity->cShape= std::make_shared<CShape>(m_enemyConfig.SR/10, degree,  sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
        entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
        entity->cCollision= std::make_shared<CCollision>(m_enemyConfig.CR);
    }
}


void  Game::spawnSpecialWeapon(std::shared_ptr<Entity> e)
{
}

void Game::sLifespan()
{

    for(auto e: m_entities.getEntities())
    {
        if (!e->cLifespan) { continue; }
        if(e->cLifespan->remaining>0)
        {
            e->cLifespan->remaining--;
        }
		if (e->isActive() && e->cLifespan->remaining > 0)
		{
			float alphaMultiplier{ static_cast<float>(e->cLifespan->remaining) / static_cast<float>(e->cLifespan->total) };

			auto fillColor{ e->cShape->circle.getFillColor() };
			sf::Color newFillColor{ fillColor.r,fillColor.g,fillColor.b, static_cast<sf::Uint8>( 255 * alphaMultiplier) };
			e->cShape->circle.setFillColor(newFillColor);

			auto outlineColor{ e->cShape->circle.getOutlineColor() };
			sf::Color newOutlineColor{ outlineColor.r,outlineColor.g,outlineColor.b, static_cast<sf::Uint8>(255 * alphaMultiplier) };
			e->cShape->circle.setOutlineColor(newOutlineColor);
		}
        else if (e->cLifespan->remaining <= 0)
            e->destroy();
    }
}
void Game::sEnemySpawner()
{
    if(m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SP)
    {
        spawnEnemy();
    }
}

