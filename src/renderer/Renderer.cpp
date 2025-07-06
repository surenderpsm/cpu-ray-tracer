#include "Renderer.h"
#include <algorithm>
#include "Material.h"

int samplesPerPixel = 1;
const int maxDepth = 5;

inline float randFloat()
{
    return static_cast<float>(rand()) / (RAND_MAX + 1.0f);
}

Renderer::Renderer(int width, int height)
    : width(width), height(height), image(width * height * 3, 0) {}

Vec3 rayColor(const Ray &r, const Scene &scene, const Vec3 &lightDir, int depth)
{
    if (depth <= 0)
        return Vec3(0, 0, 0); // Max depth reached

    HitRecord rec;
    if (scene.hit(r, 0.001f, INFINITY, rec))
    {
        Ray scattered;
        Vec3 attenuation;
        if (rec.material && rec.material->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * rayColor(scattered, scene, lightDir, depth - 1);
        }
        return Vec3(0, 0, 0); // Absorbed
    }

    // Sky gradient
    Vec3 unitDir = normalize(r.direction);
    float t = 0.5f * (unitDir.y + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

void Renderer::render(const Scene &scene, const Camera &camera, const Vec3 &lightDir)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Vec3 color(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s)
            {
                float u = (x + randFloat()) / (width - 1);
                float v = (height - 1 - y + randFloat()) / (height - 1);
                Ray r = camera.getRay(u, v);
                color += rayColor(r, scene, lightDir, maxDepth);
            }
            color /= float(samplesPerPixel);

            int index = (y * width + x) * 3;
            image[index + 0] = static_cast<uint8_t>(255.999f * std::clamp(color.x, 0.0f, 1.0f));
            image[index + 1] = static_cast<uint8_t>(255.999f * std::clamp(color.y, 0.0f, 1.0f));
            image[index + 2] = static_cast<uint8_t>(255.999f * std::clamp(color.z, 0.0f, 1.0f));
        }
    }
}

uint8_t *Renderer::getImageData()
{
    return image.data();
}
