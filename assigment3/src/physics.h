#pragma once
#include "entity.h"
#include "common.h"

class Physics {
  public:
  Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
  Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
