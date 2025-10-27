#pragma once

#include "Renderer/Texture2D.h"
#include "Renderer/Shader.h"

#include <memory>

enum PostProcessingFlags : std::uint8_t
{
    None = 0,
    Chaos = 1 << 0,
    Confuse = 1 << 1,
    Shake = 1 << 2
};

class PostProcess
{
public:
    PostProcess(const std::shared_ptr<Shader>& shader, unsigned int width, unsigned int height);
    ~PostProcess();

    void BeginFrame() const;
    void EndFrame() const;
    void Render(float deltaTime) const;

    void SetMode(int mode) { m_Mode = mode; }
    void SetConfuse(bool value) { m_Confuse = value; }
    void SetChaos(bool value) { m_Chaos = value; }
    void SetShake(bool value) { m_Shake = value; }
private:
    void Initialize();
private:
    std::shared_ptr<Shader> m_Shader{nullptr};
    std::unique_ptr<Texture2D> m_Texture{nullptr};

    unsigned int m_Width, m_Height;

    // Flags / Mode
    int m_Mode; // Post-processing effects flags - use "PostProcessingFlags"
    bool m_Confuse, m_Chaos, m_Shake;

    unsigned int m_MSFBO, m_FBO; // Multi-sampled and simple framebuffer object IDs
    unsigned int m_RBO; // Render buffer object ID
    unsigned int m_VAO; // Vertex array object
};
