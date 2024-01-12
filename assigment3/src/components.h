#pragma once

#include "animation.h"
#include "assets.h"


class Components {
  public:
     bool has = false;
};

class CTransform : public Components
{
  public:
      Vec2 pos      = {0.0, 0.0};
      Vec2 prevPos  = {0.0, 0.0};
      Vec2 scale    = {1.0, 1.0};
      Vec2 velocity = {0.0, 0.0};
      float angle   = 0;

      CTransform() {}
      CTransform(const Vec2 &p)
          : pos(p) {}
      CTransform(const Vec2 &p, const Vec2 &sp, const Vec2 &sc, float a)
          : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

class CLifeSpan : public Components
{
  public:
      int lifespan = 0;
      int frameCreated = 0;

      CLifeSpan() {}
      CLifeSpan(int duration, int frame) : lifespan(duration) , frameCreated(frame) {}
};


class CInput : public Components
{
  public:
      bool up = false;
      bool down = false;
      bool left = false;
      bool right = false;
      bool shoot = false;
      bool canShoot = true;
      bool canJump = true;
      CInput() {}
};


class CBoundingBox : public Components
{
  public:
      Vec2 size = {0.0, 0.0};
      Vec2 halfSize;

      CBoundingBox() {}
      CBoundingBox(const Vec2 &s)
          : size(s), halfSize(s.x/2, s.y/2) {}
};

class CAnimation : public Components
{
  public:
      Animation animation;
      bool repeat;
      CAnimation() {}

      CAnimation(const Animation &animation, bool r)
          : animation(animation), repeat(r) {}
};

class CGravity : public Components
{
  public:
      float gravity = 0;
      CGravity() {}
      CGravity(float g) : gravity(g){}
};

class CState : public Components
{
  public:
      std::string state = "jumping";
      CState() {}
      CState(const std::string & s) : state(s) {}
};






