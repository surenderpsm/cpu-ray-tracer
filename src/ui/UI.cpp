#include "UI.h"
#include "imgui.h"

UI::UI(UIState &state, Camera &camera, Scene &scene)
    : state(state), camera(camera), scene(scene) {}

void UI::render()
{
    ImGui::Begin("Scene Controls");

    // Field of View
    if (ImGui::SliderFloat("FOV", &state.fov, 30.0f, 120.0f))
    {
        camera = Camera(16.0f / 9.0f, state.fov);
        state.requestRender = true;
    }

    // Light direction
    if (ImGui::SliderFloat3("Light Dir", state.lightDir, -1.0f, 1.0f))
    {
        state.requestRender = true;
    }

    if (ImGui::Button("Render"))
    {
        state.requestRender = true;
    }

    ImGui::End();
}
