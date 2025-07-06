#include "UI.h"
#include "imgui.h"

UI::UI(UIState &state) : state(state) {}

void updatePreview(UIState &state)
{
    state.previewMode = true;
    state.sceneDirty = true;
    state.requestRender = true;
}

void UI::scenePanel()
{
    ImGui::Begin("Scene");

    if (ImGui::SliderFloat("FOV", &state.fov, 30.0f, 120.0f))
    {
        updatePreview(state);
    }
    if (ImGui::SliderFloat3("Light Direction", state.lightDir, -1.0f, 1.0f))
    {
        updatePreview(state);
    }

    ImGui::Separator();
    ImGui::Text("Objects");

    for (int i = 0; i < state.objects.size(); ++i)
    {
        if (ImGui::Selectable(("Sphere " + std::to_string(i)).c_str(), state.selectedObject == i))
        {
            state.selectedObject = i;
        }
    }

    if (ImGui::Button("Add Sphere"))
    {
        state.objects.push_back({Vec3(0, 0, -1), 0.5f, "lambertian", Vec3(0.8f, 0.3f, 0.3f), 0.0f});
        state.selectedObject = state.objects.size() - 1;
        updatePreview(state);
    }
    ImGui::SameLine();
    if (!state.objects.empty() && ImGui::Button("Remove Selected"))
    {
        state.objects.erase(state.objects.begin() + state.selectedObject);
        if (state.selectedObject >= state.objects.size())
            state.selectedObject = static_cast<int>(state.objects.size()) - 1;
        updatePreview(state);
    }

    if (state.selectedObject >= 0 && state.selectedObject < state.objects.size())
    {
        auto &obj = state.objects[state.selectedObject];
        if (ImGui::DragFloat3("Position", &obj.position.x, 0.1f))
            updatePreview(state);
        if (ImGui::DragFloat("Radius", &obj.radius, 0.05f, 0.01f, 10.0f))
            updatePreview(state);
    }

    ImGui::End();
}

void UI::materialPanel()
{
    ImGui::Begin("Material");

    if (state.selectedObject >= 0 && state.selectedObject < state.objects.size())
    {
        auto &obj = state.objects[state.selectedObject];

        const char *types[] = {"lambertian", "metal"};
        int currentMaterial = (obj.materialType == "metal") ? 1 : 0;

        if (ImGui::Combo("Material", &currentMaterial, types, IM_ARRAYSIZE(types)))
        {
            obj.materialType = types[currentMaterial];

            updatePreview(state);
        }

        if (ImGui::ColorEdit3("Color", &obj.color.x))
        {
            updatePreview(state);
        }

        if (obj.materialType == "metal")
        {
            if (ImGui::SliderFloat("Fuzz", &obj.fuzz, 0.0f, 1.0f))
            {
                updatePreview(state);
            }
        }
    }
    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

void UI::renderSettings()
{
    ImGui::Begin("Render Settings");
    ImGui::SliderInt("Samples", &state.samplesPerPixel, 1, 64);
    ImGui::SliderInt("Max Depth", &state.maxDepth, 1, 10);
    ImGui::SliderInt("Resolution Scale", &state.resScale, 1, 10);
    ImGui::Checkbox("Export PNG", &state.exportRequested);
    if (ImGui::Button("Render"))
    {
        state.previewMode = false;
        state.requestRender = true;
    }

    ImGui::End();
}

void UI::render()
{
    scenePanel();
    materialPanel();
    renderSettings();
}