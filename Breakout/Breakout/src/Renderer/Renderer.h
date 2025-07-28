#pragma once

#include <cstdint>
#include <glm/glm.hpp>

class Renderer
{
public:
    static void Initialize();
    static void Shutdown();

    static void SetViewport(int x, int y, int width, int height);
    static void SetClearColor(const glm::vec4& color);
    static void Clear();
private:
};
