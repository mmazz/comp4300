#include "vec2.h"
#include <math.h>

Vec2::Vec2()
{

}

Vec2::Vec2(float xin, float yin)
    : x(xin), y(yin)
{

}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x+rhs.x, y+rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x-rhs.x, y-rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(x*val, y*val);
}

Vec2 Vec2::operator / (const float val) const
{
    return Vec2(x/val, y/val);
}

bool Vec2::operator == (const Vec2& rhs) const
{
    return (x==rhs.x && y==rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return (x!=rhs.x || y!=rhs.y);
}

void Vec2::operator += (const Vec2& rhs) const
{
}

void Vec2::operator -= (const Vec2& rhs) const
{
}

void Vec2::operator *= (const float val) const
{

}

void Vec2::operator /= (const float val)const
{

}

float Vec2::dist(const Vec2& rhs) const
{
    float res = pow(x-rhs.x,2) + pow(y-rhs.y, 2);
    return sqrt(res);
}

float Vec2::length() const
{
    return 0;
}

