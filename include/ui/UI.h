#pragma once

#include "Scene.h"
#include "Camera.h"

struct UIState
{
    float fov = 90.0f;
    float lightDir[3] = {1.0f, -1.0f, -1.0f};
    bool requestRender = false;
};

class UI
{
public:
    UI(UIState &state, Camera &camera, Scene &scene);
    void render(); // call every frame

private:
    UIState &state;
    Camera &camera;
    Scene &scene;
};
