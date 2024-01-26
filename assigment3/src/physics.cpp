#include "physics.h"
#include "components.h"
#include "vec2.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
  // TODO: return the overlap rectangle size of the bounding boxes of entity a
  // and b

    float dx = abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x);
    float dy = abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y);
    float dx_BB = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x;
    float dy_BB = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y;
    float x_overlap = dx_BB - dx;
    float y_overlap = dy_BB - dy;

  return Vec2(x_overlap, y_overlap);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a,
                                 std::shared_ptr<Entity> b) {
  // TODO: return the previous overlap rectangle size of the bounding boxes of
  // entity a and b
  //       previous overlap the entity's previous position
    float dx = abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x);
    float dy = abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y);
    float dx_BB = a->getComponent<CBoundingBox>().halfSize.x + b->getComponent<CBoundingBox>().halfSize.x;
    float dy_BB = a->getComponent<CBoundingBox>().halfSize.y + b->getComponent<CBoundingBox>().halfSize.y;
    float x_overlap = dx_BB - dx;
    float y_overlap = dy_BB - dy;

  return Vec2(x_overlap, y_overlap);
}
