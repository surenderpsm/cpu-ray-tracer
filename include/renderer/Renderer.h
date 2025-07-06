#pragma once

#include "../core/Vec3.h"
#include "../core/Ray.h"
#include "../scene/Camera.h"
#include "../scene/Scene.h"
#include "../core/ImageBuffer.h"

class Renderer
{
public:
    Renderer(int width, int height);

    void render(const Scene &scene, const Camera &camera, const Vec3 &lightDir = Vec3(1, -1, -1), const int samplesPerPixel = 10, const int depth = 2);
    uint8_t *getImageData();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void resize(int width, int height)
    {
        this->height = height;
        this->width = width;
        image.resize(width * height * 3);
    }

private:
    int width, height;
    std::vector<uint8_t> image; // RGB buffer
};
