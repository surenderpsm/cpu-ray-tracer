#pragma once

#include "Hittable.h"
#include <memory>
#include <vector>

class Scene
{
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    Scene() {}

    void add(const std::shared_ptr<Hittable> &object)
    {
        objects.push_back(object);
    }

    bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const
    {
        HitRecord tempRec;
        bool hitAnything = false;
        float closestSoFar = tMax;

        for (const auto &object : objects)
        {
            if (object->hit(r, tMin, closestSoFar, tempRec))
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
};
