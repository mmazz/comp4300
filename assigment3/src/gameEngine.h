#pragma once

#include "assets.h"
#include "common.h"
#include "scene.h"
#include <memory>

class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
protected:

  sf::RenderWindow  m_window;
  Assets            m_assets;
  std::string       m_currentScene ;
  SceneMap          m_sceneMap;
  size_t            m_simulationSpeed = 1;
  size_t            m_currentFrames = 0;
  bool              m_running = true;

  void init(const std::string & path);
  void update();

  void sUserInput();

  std::shared_ptr<Scene> currentScene();

public:
  GameEngine(const std::string & path);

  void changeScene(const std::string & scenes,
                   const std::shared_ptr<Scene> & scene,
                   bool endCurrentScene = false);

  void quit();
  void run();

  sf::RenderWindow &window();
  const Assets &getAssets() const;
  bool isRunning();
};
