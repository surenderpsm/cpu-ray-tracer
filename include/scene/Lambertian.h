#pragma once
#include "Material.h"

class Lambertian : public Material
{
public:
    Vec3 albedo;

    Lambertian(const Vec3 &color) : albedo(color) {}

    virtual bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        Vec3 scatterDir = rec.normal + normalize(randomUnitVector());
        scattered = Ray(rec.point, scatterDir);
        attenuation = albedo;
        return true;
    }
};
