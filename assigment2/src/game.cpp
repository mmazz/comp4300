#include "game.h"
#include "components.h"

#include <any>
#include <complex>
#include <imgui-SFML.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>

inline extern const double PI{ 3.14159265359 };
bool collision(std::shared_ptr<Entity> &e, std::shared_ptr<Entity> &e2 )
{
    float diff_x = e->cTransform->pos.x - e2->cTransform->pos.x;
    float diff_y = e->cTransform->pos.y - e2->cTransform->pos.y;
    Vec2 diff{ diff_x , diff_y };
    float CR_sum = (e->cCollision->radius + e2->cCollision->radius);
    double collisionRadiusSQ{ CR_sum * CR_sum };
    double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };
    return distSQ < collisionRadiusSQ;
}
Vec2 velocity_diferential(Vec2 &v_diff, Vec2& x_diff)
{
    float norm = x_diff.norm2();
    float dotprod = v_diff.dot(x_diff);
    Vec2 res = x_diff * (dotprod / norm);
    return res;
}


Game::Game(const std::string & config)
{
    init(config);
}
void Game::updateText(const sf::Text &text)
{
    int cooldown = 4*m_enemyConfig.SP;
    if(m_currentFrame - m_lastSpecialBulletSpawnTime<4*m_enemyConfig.SP)
        cooldown = (m_currentFrame - m_lastSpecialBulletSpawnTime)%(4*m_enemyConfig.SP);
    m_text.setString("Score: " + std::to_string(m_score) + "\n"
                    "Special weapon cooldown: " + std::to_string(100*cooldown/(4*m_enemyConfig.SP))+"%");
}

void Game::init(const std::string & path)
{
    LoadConfig(path);
    if(m_fullScreen)
        m_window.create(sf::VideoMode(m_width, m_height), "Assigment 2", sf::Style::Fullscreen);
    else
        m_window.create(sf::VideoMode(m_width, m_height), "Assigment 2", sf::Style::Titlebar);

    m_window.setFramerateLimit(m_frameRate);
    updateText(m_text);
	m_text.setPosition(0, 0);
    updateText(m_text);

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

            updateText(m_text);
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
    entity->cScore     = std::make_shared<CScore>(0.0f);

    m_player = entity;
}

void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity("enemy");
    float ex = rand() % (m_width-2);
    float ey = rand() % (m_height-2);
    float speed = rand() % (int)(1+ m_enemyConfig.SMAX - m_enemyConfig.SMIN)+m_enemyConfig.SMIN;
    float angle = rand() % (360)*PI/180.0f;
    float Xchange = cos( angle);
    float Ychange = sin(angle);
    Vec2 velocity{Xchange*speed, Ychange*speed};
    float r = rand() % 255;
    float g = rand() % 255;
    float b = rand() % 255;
    float  shape = rand() % (1 + m_enemyConfig.VMAX-m_enemyConfig.VMIN)+m_enemyConfig.VMIN;
    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), velocity, 0.0f);
    entity->cShape= std::make_shared<CShape>(m_enemyConfig.SR, shape, sf::Color(r, g, b),
            sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
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


// hacer una funcion booleada de deteccion de choque
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

            updateText(m_text);
            spawnSmallEnemies(enemy);
            enemy->destroy();
            m_player->destroy();
            spawnPlayer();

        }

    }
    for (auto bullet: m_entities.getEntities("specialBullet"))
        {
            for (auto enemy: m_entities.getEntities("enemy"))
            {
                Vec2 diff{ enemy->cTransform->pos.x - bullet->cTransform->pos.x , enemy->cTransform->pos.y - bullet->cTransform->pos.y };

                double collisionRadiusSQ{ (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy->cCollision->radius) };
                double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

                if (distSQ < collisionRadiusSQ)
                {
                    //Updates the score
                    m_score += enemy->cShape->circle.getPointCount()*100;
                    updateText(m_text);
                    spawnSmallEnemies(enemy);
                    enemy->destroy();
                }
            }

            for (auto enemy: m_entities.getEntities("smallEnemy"))
            {
                Vec2 diff{ enemy->cTransform->pos.x - bullet->cTransform->pos.x , enemy->cTransform->pos.y - bullet->cTransform->pos.y };

                double collisionRadiusSQ{ (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy->cCollision->radius) };
                double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

                if (distSQ < collisionRadiusSQ)
                {
                    //Updates the score
                    m_score += enemy->cShape->circle.getPointCount()*200;
                    updateText(m_text);
                    enemy->destroy();
                }
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
                m_score += enemy->cShape->circle.getPointCount()*100;
                updateText(m_text);
                spawnSmallEnemies(enemy);
                enemy->destroy();
                bullet->destroy();
            }
        }

        for (auto enemy: m_entities.getEntities("smallEnemy"))
        {
            Vec2 diff{ enemy->cTransform->pos.x - bullet->cTransform->pos.x , enemy->cTransform->pos.y - bullet->cTransform->pos.y };

            double collisionRadiusSQ{ (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy->cCollision->radius) };
            double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

            if (distSQ < collisionRadiusSQ)
            {
                //Updates the score
                m_score += enemy->cShape->circle.getPointCount()*200;
                updateText(m_text);
                enemy->destroy();
                bullet->destroy();
            }
        }
    }
    for (auto e: m_entities.getEntities("enemy"))
    {
        for (auto enemy: m_entities.getEntities("enemy"))
        {
            if(e!=enemy)
            {
                // Asco.... rehacerlo
                if (collision(e,enemy))
                {
                    float diff_x = e->cTransform->pos.x - enemy->cTransform->pos.x;
                    float diff_y = e->cTransform->pos.y - enemy->cTransform->pos.y;
                    Vec2 diff{ diff_x , diff_y };
                    float length = diff.length();
                    float CR_sum = (e->cCollision->radius + enemy->cCollision->radius);
                    float diff_intersection = length-CR_sum;
                    diff.normalize();
                    diff = diff*diff_intersection/2.0f;
                    double collisionRadiusSQ{ CR_sum * CR_sum };
                    double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };
                    // Los acomodo para que no se esten tocando
                    e->cTransform->pos.x -= diff.x;
                    e->cTransform->pos.y -= diff.y;
                    enemy->cTransform->pos.x += diff.x;
                    enemy->cTransform->pos.y += diff.y;

                    e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
                    enemy->cShape->circle.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
               //     // ahora los hago rebotar
                    Vec2 rel_vel = enemy->cTransform->velocity - e->cTransform->velocity;
                    Vec2 rel_vel2 = e->cTransform->velocity - enemy->cTransform->velocity;
                    Vec2 rel_pos = enemy->cTransform->pos - e->cTransform->pos;
                    Vec2 rel_pos2 = e->cTransform->pos - enemy->cTransform->pos;


                    enemy->cTransform->velocity -= velocity_diferential(rel_vel, rel_pos);
                    e->cTransform->velocity     -= velocity_diferential(rel_vel2, rel_pos2);

                }
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

        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(ImGui::GetIO().WantCaptureMouse) { continue; }

            if(event.mouseButton.button == sf::Mouse::Left)
            {
                const Vec2 mousePos{ static_cast<float>(sf::Mouse::getPosition(m_window).x), static_cast<float>(sf::Mouse::getPosition(m_window).y) };
                spawnBullet(m_player, mousePos);
            }

            if(event.mouseButton.button == sf::Mouse::Right)
            {
                const Vec2 mousePos{ static_cast<float>(sf::Mouse::getPosition(m_window).x), static_cast<float>(sf::Mouse::getPosition(m_window).y) };
                spawnSpecialWeapon(m_player, mousePos);
            }
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
    float angleDelta = 360.0f/degree * PI / 180.0 ;
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
        Vec2 velocity{Xchange*speed, Ychange*speed};

        entity->cTransform = std::make_shared<CTransform>(Vec2(posX, posY), velocity, 0.0f);

        entity->cShape= std::make_shared<CShape>(m_enemyConfig.SR/10, degree,  sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
        entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
        entity->cCollision= std::make_shared<CCollision>(m_enemyConfig.CR);
    }
}


void  Game::spawnSpecialWeapon(std::shared_ptr<Entity> e, const Vec2 & target)
{
    if(m_currentFrame - m_lastSpecialBulletSpawnTime > 4*m_enemyConfig.SP)
    {
        auto entity = m_entities.addEntity("specialBullet");
        float speed = m_bulletConfig.S;
        Vec2 difference{ target.x - e->cTransform->pos.x, target.y - e->cTransform->pos.y };
        difference.normalize();
        Vec2 velocity{m_bulletConfig.S * difference.x, m_bulletConfig.S * difference.y};
        entity->cTransform = std::make_shared<CTransform>(Vec2(e->cTransform->pos.x, e->cTransform->pos.y), velocity, 0.0f);
        float  shape = m_bulletConfig.V;
        entity->cShape= std::make_shared<CShape>(3*m_bulletConfig.SR, shape, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
        entity->cLifespan = std::make_shared<CLifespan>(15*m_bulletConfig.L);
        entity->cCollision= std::make_shared<CCollision>(3*m_bulletConfig.CR);
        m_lastSpecialBulletSpawnTime = m_currentFrame;
    }
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

