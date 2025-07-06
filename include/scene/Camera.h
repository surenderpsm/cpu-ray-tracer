#pragma once

#include "Ray.h"

class Camera
{
public:
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;

    Camera(float aspectRatio = 16.0f / 9.0f, float fov = 90.0f)
    {
        float theta = fov * M_PI / 180.0f;
        float h = std::tan(theta / 2);
        float viewportHeight = 2.0f * h;
        float viewportWidth = aspectRatio * viewportHeight;

        origin = Vec3(0, 0, 0);
        horizontal = Vec3(viewportWidth, 0, 0);
        vertical = Vec3(0, viewportHeight, 0);
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, 1);
    }

    Ray getRay(float u, float v) const
    {
        return Ray(origin,
                   lowerLeftCorner + u * horizontal + v * vertical - origin);
    }
};
