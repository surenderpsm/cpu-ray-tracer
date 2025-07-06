#pragma once

#include "Vec3.h"

class Ray
{
public:
  Vec3 origin;
  Vec3 direction;

  Ray() {}
  Ray(const Vec3 &origin, const Vec3 &direction)
      : origin(origin), direction(direction) {}

  Vec3 at(float t) const { return origin + t * direction; }
};
