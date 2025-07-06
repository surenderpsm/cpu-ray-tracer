#pragma once

#include "../core/Ray.h"
#include "../core/Vec3.h"
#include "Hittable.h"

class Material
{
public:
    virtual ~Material() = default;

    // Given an incoming ray, hit record, return scattered ray and attenuation
    virtual bool scatter(
        const Ray &in,
        const HitRecord &rec,
        Vec3 &attenuation,
        Ray &scattered) const = 0;
};
