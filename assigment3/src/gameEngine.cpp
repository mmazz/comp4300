#include "gameEngine.h"
#include "assets.h"
#include "scenePlay.h"
#include "sceneMenu.h"

#include <iostream>

GameEngine::GameEngine(const std::string &path)
{
    init(path);
}

void GameEngine::init(const std::string &path)
{
  m_assets.loadFromFile(path);

  m_window.create(sf::VideoMode(1080, 810), "Game Mechaman");
  m_window.setFramerateLimit(60);
  m_currentScene = "MENU";
  changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
  return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
    return m_running & window().isOpen();
}

sf::RenderWindow &GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }
}

void GameEngine::sUserInput()
{
  sf::Event event{};
  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      quit();

    if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::X)
      {
        sf::Texture texture;
        texture.create(m_window.getSize().x, m_window.getSize().y);
        texture.update(m_window);
        if (texture.copyToImage().saveToFile("../include/screenshots/test.png"))
          std::cout << "screenshot saved to "
                    << "../include/screenshots/test.png" << std::endl;
      }
    }
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
    {
      // if the current scene does not have an action associated with this key,
      // skip the event
      if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
      {
          continue;
      }
      // determinate start or end action whether it was pres or release
      const std::string actionType = (event.type == sf::Event::KeyPressed ? "START" : "END");
      // look up the action and send the action to the scene
      currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
    }
  }
}

void GameEngine::changeScene(const std::string & sceneName, const std::shared_ptr<Scene> &scene,
                             bool endCurrentScene)
{
  m_currentScene = sceneName;

  if (scene)
  {
    m_sceneMap[sceneName] = scene;
  }
  else
  {
    if(m_sceneMap.find(sceneName) == m_sceneMap.end())
    {
        std::cerr << "Warning: scene does not exist: " << sceneName << std::endl;
        return;
    }
  }
  if(endCurrentScene)
  {
      m_sceneMap.erase(m_sceneMap.find(m_currentScene));
  }
}

const Assets &GameEngine::getAssets() const
{
    return m_assets;
}

void GameEngine::quit()
{
    m_running = false;
}


void GameEngine::update()
{
    sUserInput();
    m_sceneMap.at(m_currentScene)->update();
    m_currentFrames++;
}
