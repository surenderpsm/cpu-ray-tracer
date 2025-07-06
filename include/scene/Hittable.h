#pragma once

#include "Ray.h"

struct Material;

struct HitRecord
{
    Vec3 point;
    Vec3 normal;
    float t;
    bool frontFace;
    const Material *material;

    void set_face_normal(const Ray &ray, const Vec3 &outwardNormal)
    {
        frontFace = dot(ray.direction, outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray &r, float tMin, float tMax,
                     HitRecord &rec) const = 0;
};
