#pragma once

#include "../scene/Scene.h"
#include "../scene/Camera.h"
#include <string>
#include <vector>

struct EditableSphere
{
    Vec3 position;
    float radius;
    std::string materialType; // "lambertian", "metal"
    Vec3 color;
    float fuzz = 0.0f;
};

struct UIState
{
    float fov = 90.0f;
    float lightDir[3] = {1.0f, -1.0f, -1.0f};
    int samplesPerPixel = 8;
    int maxDepth = 5;
    bool requestRender = true;
    bool sceneDirty = false;
    int renderWidth = 1000;
    int renderHeight = 562;
    int resScale = 1;

    bool renderQueued = false;
    bool isRendering = true;

    bool exportRequested = false;

    bool previewMode = true;

    std::vector<EditableSphere> objects;
    int selectedObject = 0;
};

class UI
{
public:
    UI(UIState &state);
    void render();

private:
    UIState &state;

    // helpers
    void scenePanel();
    void materialPanel();
    void renderSettings();
};