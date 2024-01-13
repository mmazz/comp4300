#include "sceneMenu.h"
#include "assets.h"
#include "components.h"
#include "gameEngine.h"
#include "scenePlay.h"
#include "action.h"
#include "common.h"
#include <SFML/Window/Keyboard.hpp>

Scene_Menu::Scene_Menu(GameEngine *game)
    : Scene(game)
{

 std::cout<< "scene menu creation" << std::endl;
    init();
}

void Scene_Menu::init() {

    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "PLAY");
    m_title = "Mega Mario";
    m_menuStrings.push_back("Level  1");
    m_menuStrings.push_back("Level  2");
    m_menuStrings.push_back("Level  3");

    m_levelPaths.push_back("include/levels/level1.txt");
    m_levelPaths.push_back("include/levels/level2.txt");
    m_levelPaths.push_back("include/levels/level3.txt");

    m_menuText.setFont(m_game->getAssets().getFont("Pixeled"));
    m_menuText.setCharacterSize(64);
}

void Scene_Menu::update()
{
    sUserInput();
  m_entityManager.update();
  sRender();
}

void Scene_Menu::sUserInput()
{
    sf::Event event{};
    while (m_game->window().pollEvent(event))
    {

        if (event.type == sf::Event::Closed)
        {
            m_game->quit();
        }
        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed)
        {
                    std::cout << "press " << std::endl;
            switch (event.key.code)
            {
                case sf::Keyboard::Escape:
                    {
                        m_game->quit();
                        break;
                }
                case sf::Keyboard::W:
                {
                    std::cout << "w " << std::endl;
                    if (m_selectedMenuIndex > 0)
                    {
                        m_selectedMenuIndex--;
                    }
                    else
                    {
                        m_selectedMenuIndex = m_menuStrings.size() - 1;
                    }
                    break;
                }
                case sf::Keyboard::S:
                {
                    m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
                    break;
                }
                case sf::Keyboard::D:
                {
                    std::cout << "D " << std::endl;
                    m_game->changeScene("PLAY",
                    std::make_shared<Scene_Play>(
                    m_game, m_levelPaths.at(m_selectedMenuIndex)));
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}

void Scene_Menu::sRender() {
  m_game->window().clear(sf::Color(100, 27, 0));
  // drawing the game title in the top-left of the screen
  m_menuText.setString(m_title);
  m_menuText.setCharacterSize(32);
  m_menuText.setFillColor(sf::Color::Black);
  m_menuText.setPosition(sf::Vector2f(m_game->window().getSize().x / 2 -
                                          m_menuText.getCharacterSize() * 3,
                                      10));
  m_game->window().draw(m_menuText);
  // draw all the menu options
  for (size_t i = 0; i < m_menuStrings.size(); i++) {
    m_menuText.setString(m_menuStrings.at(i));
    m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White
                                                     : sf::Color::Black);
    m_menuText.setPosition(sf::Vector2f(m_game->window().getSize().x / 2 -
                                            m_menuText.getCharacterSize() * 2,
                                        250 + i * 72));
    m_game->window().draw(m_menuText);
  }

  m_menuText.setString(
      "Left :  A        S :  Down        Up : W        D : Do");
  m_menuText.setFillColor(sf::Color::Black);
  m_menuText.setCharacterSize(12);
  m_menuText.setPosition(
      sf::Vector2f(m_menuText.getCharacterSize(),
                   height() - m_menuText.getCharacterSize() * 2));
  m_game->window().draw(m_menuText);

  m_game->window().display();
}

void Scene_Menu::onEnd()
{

}

void Scene_Menu::sDoAction(const Action &action)
{
    if(action.type() == "START")
    {
                    std::cout << action.name()<< std::endl;
        if(action.name() == "UP")
        {
            if (m_selectedMenuIndex > 0) {m_selectedMenuIndex--;}
            else {m_selectedMenuIndex = m_menuStrings.size() - 1;}
        }
        else  if (action.name() == "DOWN")
        {
            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
        }
        else if (action.name() == "PLAY")
        {
            m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
            std::cout << "PLAY" << std::endl;
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}
