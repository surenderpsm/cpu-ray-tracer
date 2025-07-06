#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Sphere.h"
#include "Scene.h"
#include "Renderer.h"
#include "ImageBuffer.h"
#include "UI.h"
#include "Lambertian.h"
#include "Metal.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image_resize.h"

// ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>
#include <memory>
#include <vector>

const int BASE_WIDTH = 200;
const int BASE_HEIGHT = 112;
const int DISPLAY_WIDTH = 1000;
const int DISPLAY_HEIGHT = 562;

GLuint createTexture(int width, int height, uint8_t *data)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return tex;
}

void updateTexture(GLuint texture, int width, int height, uint8_t *data)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
}

int main()
{
    // Init GLFW + GLAD
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(DISPLAY_WIDTH, DISPLAY_HEIGHT, "CPU Ray Tracer", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    // Init UI state and scene
    UIState uiState;
    uiState.objects.push_back({Vec3(0, 0, -1), 0.5f, "lambertian", Vec3(0.8f, 0.3f, 0.3f), 0.0f});
    uiState.objects.push_back({Vec3(0, -100.5f, -1), 100.0f, "lambertian", Vec3(0.8f, 0.8f, 0.0f), 0.0f});
    uiState.objects.push_back({Vec3(1, 0, -1), 0.5f, "metal", Vec3(0.8f, 0.8f, 0.8f), 0.1f});
    uiState.sceneDirty = true;
    UI ui(uiState);

    Scene scene;
    Camera camera(16.0f / 9.0f, uiState.fov);

    Renderer renderer(BASE_WIDTH, BASE_HEIGHT);

    std::vector<uint8_t> finalBuffer(DISPLAY_WIDTH * DISPLAY_HEIGHT * 3);
    GLuint texture = createTexture(DISPLAY_WIDTH, DISPLAY_HEIGHT, finalBuffer.data());

    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI
        ui.render();
        if (uiState.requestRender && uiState.previewMode)
        {
            uiState.requestRender = false;
            uiState.isRendering = true;
        }
        if (uiState.requestRender)
        {
            uiState.requestRender = false;
            uiState.renderQueued = true;
        }
        else if (uiState.renderQueued)
        {
            uiState.renderQueued = false;
            uiState.isRendering = true;
        }
        else if (uiState.isRendering)
        {
            // Rebuild scene if dirty
            if (uiState.sceneDirty)
            {
                scene.clear();
                for (const auto &obj : uiState.objects)
                {
                    Material *mat;
                    if (obj.materialType == "metal")
                        mat = new Metal(obj.color, obj.fuzz);
                    else
                        mat = new Lambertian(obj.color);
                    scene.add(std::make_shared<Sphere>(obj.position, obj.radius, mat));
                }
                camera = Camera(16.0f / 9.0f, uiState.fov);
                uiState.sceneDirty = false;
            }

            // Render at scaled res
            int scale = uiState.previewMode ? 1 : uiState.resScale;
            int renderW = BASE_WIDTH * scale;
            int renderH = BASE_HEIGHT * scale;

            renderer.resize(renderW, renderH);
            Vec3 lightDir(uiState.lightDir[0], uiState.lightDir[1], uiState.lightDir[2]);
            if (uiState.previewMode)
            {
                uiState.samplesPerPixel = 8;
                uiState.maxDepth = 2;
            }
            renderer.render(scene, camera, lightDir, uiState.samplesPerPixel, uiState.maxDepth);

            stbir_resize_uint8(renderer.getImageData(), renderW, renderH, 0,
                               finalBuffer.data(), DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 3);

            updateTexture(texture, DISPLAY_WIDTH, DISPLAY_HEIGHT, finalBuffer.data());
            uiState.isRendering = false;
            if (!uiState.previewMode && uiState.exportRequested)
            {
                stbi_write_png("output.png", DISPLAY_WIDTH, DISPLAY_HEIGHT, 3, finalBuffer.data(), DISPLAY_WIDTH * 3);
                std::cout << "Exported to output.png" << std::endl;
            }
        }

        // Draw Image
        ImGui::Begin("Render Output");
        ImGui::Image((void *)(intptr_t)texture, ImVec2(DISPLAY_WIDTH, DISPLAY_HEIGHT));
        if (uiState.isRendering)
        {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Rendering full image...");
        }
        ImGui::End();

        // Final draw
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}