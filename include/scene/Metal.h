#pragma once
#include "Material.h"

class Metal : public Material
{
public:
    Vec3 albedo;
    float fuzz;

    Metal(const Vec3 &color, float fuzziness) : albedo(color), fuzz(fuzziness < 1 ? fuzziness : 1) {}

    virtual bool scatter(const Ray &in, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = reflect(normalize(in.direction), rec.normal);
        scattered = Ray(rec.point, reflected + fuzz * randomUnitVector());
        attenuation = albedo;
        return (dot(scattered.direction, rec.normal) > 0);
    }
};
