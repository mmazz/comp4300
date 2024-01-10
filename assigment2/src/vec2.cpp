#include "vec2.h"
#include <math.h>
inline extern const double PI{ 3.14159265359 };

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

void Vec2::operator += (const Vec2& rhs)
{
    x = x + rhs.x;
    y = y + rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
    x = x - rhs.x;
    y = y - rhs.y;
}

void Vec2::operator *= (const float val)
{
    x = x * val;
    y = y * val;

}

void Vec2::operator /= (const float val)
{
    if(val!=0)
    {
        x = x * val;
        y = y * val;
    }
    else
        std::cerr << "Cant divide by zero" << std::endl;

}
float Vec2::dot(const Vec2& b) const
{
    float res;
    float aNorm = this->length();
    float bNorm = b.length();
    float angle = this->angle(b);
    res = this->x * b.x + this->y * b.y;
    return res;
}
void Vec2::normalize()
{
    float L = length();
    x = x/L;
    y = y/L;
}

float Vec2::length() const
{
    return sqrtf(x*x+y*y);
}
float Vec2::norm2() const
{
    return x*x+y*y;
}
float Vec2::dist(const Vec2& rhs) const
{
    float res = pow(x-rhs.x,2) + pow(y-rhs.y, 2);
    return sqrtf(res);
}
float Vec2::angle(const Vec2& rhs) const
{
    Vec2 Diff = Vec2(x - rhs.x, y - rhs.y);
    float res = atan2f(Diff.y, Diff.x)*PI/180;
    return res;
}
