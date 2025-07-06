#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
    Vec3 center;
    float radius;
    const Material *mat;

    Sphere(const Vec3 &center, float radius, const Material *mat)
        : center(center), radius(radius), mat(mat) {}

    Sphere() {}
    Sphere(const Vec3 &center, float radius) : center(center), radius(radius) {}

    virtual bool hit(const Ray &r, float tMin, float tMax,
                     HitRecord &rec) const override
    {
        Vec3 oc = r.origin - center;
        float a = dot(r.direction, r.direction);
        float half_b = dot(oc, r.direction);
        float c = dot(oc, oc) - radius * radius;

        float discriminant = half_b * half_b - a * c;
        if (discriminant < 0)
            return false;
        float sqrtD = std::sqrt(discriminant);

        // Find nearest root in acceptable range
        float root = (-half_b - sqrtD) / a;
        if (root < tMin || root > tMax)
        {
            root = (-half_b + sqrtD) / a;
            if (root < tMin || root > tMax)
                return false;
        }
        rec.material = mat;
        rec.t = root;
        rec.point = r.at(rec.t);
        Vec3 outwardNormal = (rec.point - center) / radius;
        rec.set_face_normal(r, outwardNormal);
        return true;
    }
};
