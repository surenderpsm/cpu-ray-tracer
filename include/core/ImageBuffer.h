#pragma once

#include <vector>
#include <cstdint>

class ImageBuffer
{
public:
    ImageBuffer(int width, int height)
        : width(width), height(height), pixels(width * height * 3, 0) {}

    uint8_t *data() { return pixels.data(); }
    const uint8_t *data() const { return pixels.data(); }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void resize(int newWidth, int newHeight)
    {
        width = newWidth;
        height = newHeight;
        pixels.resize(width * height * 3);
    }

    void clear(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0)
    {
        for (int i = 0; i < width * height; ++i)
        {
            pixels[i * 3 + 0] = r;
            pixels[i * 3 + 1] = g;
            pixels[i * 3 + 2] = b;
        }
    }

private:
    int width, height;
    std::vector<uint8_t> pixels;
};
