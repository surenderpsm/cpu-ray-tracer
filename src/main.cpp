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
// ImGui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>
#include <memory>

const int WIDTH = 800;
const int HEIGHT = 450;

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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CPU Ray Tracer", nullptr, nullptr);
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

    // Init Scene
    Scene scene;
    auto red = new Lambertian(Vec3(0.8f, 0.3f, 0.3f));
    auto ground = new Lambertian(Vec3(0.8f, 0.8f, 0.0f));
    auto metal = new Metal(Vec3(0.8f, 0.8f, 0.8f), 0.1f);

    scene.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5, red));
    scene.add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100, ground));
    scene.add(std::make_shared<Sphere>(Vec3(1, 0, -1), 0.5, metal));

    Camera camera(16.0f / 9.0f, 90.0f);
    Renderer renderer(WIDTH, HEIGHT);
    renderer.render(scene, camera);

    GLuint texture = createTexture(renderer.getWidth(), renderer.getHeight(), renderer.getImageData());

    // UI Setup
    UIState uiState;
    UI ui(uiState, camera, scene);

    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI rendering
        ui.render();

        // Re-render scene if UI triggered it
        if (uiState.requestRender)
        {
            Vec3 lightDir(uiState.lightDir[0], uiState.lightDir[1], uiState.lightDir[2]);
            renderer.render(scene, camera, lightDir);

            updateTexture(texture, renderer.getWidth(), renderer.getHeight(), renderer.getImageData());
            uiState.requestRender = false;
        }

        // Draw image to ImGui
        ImGui::Begin("Render Output");
        ImGui::Image((void *)(intptr_t)texture, ImVec2(WIDTH, HEIGHT));
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
