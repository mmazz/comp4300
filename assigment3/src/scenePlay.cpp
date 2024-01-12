#include "scenePlay.h"
#include "physics.h"
#include "assets.h"
#include "gameEngine.h"
#include "components.h"
#include "action.h"

#include <iostream>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath)
{
  init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath)
{
  registerAction(sf::Keyboard::Z     , "PAUSE");
  registerAction(sf::Keyboard::Escape, "QUIT");
  registerAction(sf::Keyboard::T     , "TOGGLE_TEXTURE");
  registerAction(sf::Keyboard::C     , "TOGGLE_COLLISION");
  registerAction(sf::Keyboard::P     , "TOGGLE_GRID");

  //  TODO: Register all other gameplay Actions

  m_gridText.setCharacterSize(12);
  m_gridText.setFont(m_game->getAssets().getFont("Tech"));
  loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY,
                                std::shared_ptr<Entity> entity)
{
  //      TODO: This function takes in a grid (x,y) position and an Entity
  //            Return a Vec2 indicating where the CENTER position of the Entity
  //            should be You must use the Entity's Animation size to position
  //            it correctly The size of the grid width and height is stored in
  //            m_gridSize.x and m_gridSize.y The up-left corner of the
  //            Animation should align with the bottom left of the grid cell

  return Vec2(0, 0);
}

void Scene_Play::loadLevel(const std::string &filePath)
{
  //  reset the entity manager every time we load a level
  m_entityManager = EntityManager();
  // TODO : read in the level file and add the appropriate entities
  //        use the PlayerConfig struct m_playerConfig to store player
  //        proprieties this struct is defined at the top of Scene_Play.h

  // NOTE : all the code below is a sample code which shows you how to
  //        set up and use entities with the new syntax, it should be removed

  spawnPlayer();

    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT : always add the CAnimation component first so that gridToMidPixel can compute correctly
    brick->addComponents<CAnimation>(m_game->getAssets().getAnimation("Brick"), true);
    brick->addComponents<CTransform>(Vec2(96, 480));
    // NOTE : you finally code should position the entity with the grid x,y position read from the file
    //  brick->addComponents<CTransform>(gridToMidPixel(gridX, gridY, brick))

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
      std::cout << "This could be a good way of identifying if a tile is a brick!" << std::endl;



    auto block = m_entityManager.addEntity("title");
    block->addComponents<CAnimation>(m_game->getAssets().getAnimation("Block"),
    true); block->addComponents<CTransform>(Vec2(224, 480));
  //  // add a bounding box, this will now show up if we press the 'C' key
    block->addComponents<CBoundingBox>(m_game->getAssets().getAnimation("block").getSize());

    auto question = m_entityManager.addEntity("tile");
    question->addComponents<CAnimation>(m_game->getAssets().getAnimation("Question"),true);
    question->addComponents<CTransform>(Vec2(352, 480));

  // NOTE : THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
  //        Components are now returned as reference rather than pointers
  //        if you do not specify a reference variable type, it will COPY the
  //        component here is an example
  //
  //        This will COPY the transform into the variable 'transform1' - it's
  //        INCORRECT any changes you make to transform1 will not be changed
  //        inside the entity auto transform1 = entity->get<CTransform>();
  //
  //        This will REFERENCE the transform with the variable 'transform2' -
  //        it's CORRECT Now any changes you make to transform2 will be changed
  //        inside the entity auto & transform2 = entity->get<CTransform>()
}


void Scene_Play::spawnPlayer()
{
  // here is a sample player entity which you can use to construct other
  // entities

  m_player = m_entityManager.addEntity("player");
  m_player->addComponents<CAnimation>(m_game->getAssets().getAnimation("Stand"), true);
  m_player->addComponents<CTransform>(Vec2(224, 352));
  m_player->addComponents<CBoundingBox>(Vec2(48, 48));
 // m_player->addComponents<CGravity>(0.1);

  // TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnEnemy()
{
  // TODO(due: Friday): implement all the spawn enemy stuff
  // outh use the goomba animation and be added as Tile entity
  // see the current implementation of how the goomba is built
  // form the loadLevel method
}

void Scene_Play::spawnBullet(const std::shared_ptr<Entity> &entity)
{
  // TODO: this should spawn a bullet at the given entity, going in the
  // direction the entity is facing
}


void Scene_Play::update()
{
  m_entityManager.update();
  // TODO: implement pause functionality
  sMovement();
  sCollision();
  sLifespan();
  sAnimation();
  sRender();
}

void Scene_Play::sMovement()
{
  // TODO: implement player movement / jumping based on its CInput component
  // TODO: implement gravity's effect on the player
  // TODO: implement the maximum player speed in both X and Y directions
  // NOTE: setting an  entity's scale.x to -1/1 will make it face to the
  // left/right
  }

void Scene_Play::sLifespan()
{
  // TODO: check lifespan of entities that have them, and destroy them if they
  // go over

}

void Scene_Play::sCollision()
{
  // REMEMBER: SFML's (0,0) position is in the TOP-Left corner
  //           this means jumping will have negative y-component
  //           and gravity will have positive y-component
  //           Also, something BELOW something else will have a y value GREATER
  //           than it Also, something ABOVE something else will have a y value
  //           LOWER than it
  // TODO: implement Physics::GetOverlap() function, use it inside this function
  // TODO: implement bullet / tile collisions
  //       destroy the tile if it has a Brick animation
  // TODO: implement player / tile collisions and resolutions
  //       update the CState component of the player to store whether
  //       used by the Animation system
  // TODO: check to see if the player has fallen down a hole (y >height())
  // TODO: Dont let the player walk off the left side of the map

}

void Scene_Play::sDoAction(const Action &action)
{
  if (action.type() == "START")
  {
         if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
    else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision= !m_drawCollision; }
    else if (action.name() == "TOGGLE_GRID") { m_drawGrid= !m_drawGrid; }
    else if (action.name() == "PAUSE") {setPaused(!m_paused); }
    else if (action.name() == "QUIT") {onEnd(); }
    else if (action.name() == "UP")
    {
        m_player->getComponent<CInput>().up = true;

    }
  }
  else if (action.type() == "END")
  {
      if (action.name() == "UP")
      {
          m_player->getComponent<CInput>().up = false;
      }
  }
}
void Scene_Play::sAnimation()
{
  // TODO: complete the Animation class code first

  // TODO: set the animation of the player based on its CState component
  // TODO: for each entity with an animation, call
  // entity->getComponent<CAnimation>().animation.update();
  //       if the animation is not repeated, and it has ended, destroy the
  //       entity

}

void Scene_Play::onEnd()
{
  // TODO: when the scene ends, change back to the MENU scene
  //       use m_game->changeState(correct params)
}

void Scene_Play::sRender()
{
  // color the background darker, so you know that the game is paused
  if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255));}
  else {m_game->window().clear(sf::Color(50, 50, 150)); }

  // set the viewport of the window to be centered on the player if its far
  // enough right
  auto &pPos = m_player->getComponent<CTransform>().pos;
  float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
  sf::View view = m_game->window().getView();
  view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
  m_game->window().setView(view);

  // draw all Entity textures / animations
  if (m_drawTextures)
  {
    for (auto &e: m_entityManager.getEntities())
    {
      auto &transform = e->getComponent<CTransform>();
      if (e->hasComponent<CAnimation>())
      {
        auto &animation = e->getComponent<CAnimation>().animation;
        animation.getSprite().setRotation(transform.angle);
        animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
        animation.getSprite().setScale(transform.scale.x, transform.scale.y);
        m_game->window().draw(animation.getSprite());
      }
    }
  }

  // draw all Entity collision bounding boxes with a rectangle-shape
  if (m_drawCollision)
  {
    for (auto &e: m_entityManager.getEntities())
    {
      if (e->hasComponent<CBoundingBox>())
      {
        auto &box = e->getComponent<CBoundingBox>();
        auto &transform = e->getComponent<CTransform>();
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
        rect.setOrigin(sf::Vector2f(box.size.x / 2, box.size.y / 2));
        rect.setPosition(transform.pos.x, transform.pos.y);
        rect.setFillColor(sf::Color(0, 0, 0, 0));
        rect.setOutlineColor(sf::Color(255, 255, 255, 255));
        rect.setOutlineThickness(1);
        m_game->window().draw(rect);
      }
    }
  }

  // draw the grid so that studetns can easily debug
  if (m_drawGrid)
  {
      float leftX = m_game->window().getView().getCenter().x - width()/2;
      float rightX = leftX + width() + m_gridSize.x;
      float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

    for (float x = nextGridX; x < rightX; x += m_gridSize.x)
      drawLine(Vec2(x, 0), Vec2(x, height()));

    for (float y = 0; y < height(); y += m_gridSize.y)
    {
      drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

      for (float x = nextGridX; x < rightX; x += m_gridSize.x)
      {
          std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
          std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
          m_gridText.setString("(" + xCell + "," + yCell+ ")");
          m_gridText.setPosition(x+3, height() - y - m_gridSize.y + 2);
          m_game->window().draw(m_gridText);
      }
    }
  }
  m_game->window().display();
}
