cmake_minimum_required(VERSION 3.10)
set(GLFW_BUILD_WAYLAND OFF)


project(cpu-raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW, GLAD, ImGui assumed in vendor/
add_subdirectory(vendor/glfw)

# Source files
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.h
)

add_executable(raytracer ${SRC_FILES})

# GLAD
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# ImGui
file(GLOB IMGUI_SOURCES
    vendor/imgui/*.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC vendor/imgui vendor/imgui/backends vendor/glfw/include)

# Includes
target_include_directories(raytracer PRIVATE
    include/core
    include/renderer
    include/scene
    include/ui
    vendor/glad/include
    vendor/glfw/include
    vendor/imgui
    vendor/json
    vendor/stb/include
)

# Link everything
target_link_libraries(raytracer PRIVATE glfw glad imgui)
