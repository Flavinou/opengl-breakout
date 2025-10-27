#include "PostProcessing.h"

#include "Core.h"

#include <glad/glad.h>

#include <iostream>

PostProcess::PostProcess(const std::shared_ptr<Shader>& shader, unsigned int width, unsigned int height)
    : m_Shader(shader)
    , m_Width(width), m_Height(height)
    , m_Mode(PostProcessingFlags::None)
{
    // Initialize renderbuffer/framebuffer object
    glGenFramebuffers(1, &m_MSFBO);
    glGenFramebuffers(1, &m_FBO);
    glGenRenderbuffers(1, &m_RBO);

    // Initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    GLint maxSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, maxSamples, GL_RGB, m_Width, m_Height); // allocate storage for render buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RBO); // attach MS render buffer object to framebuffer
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSING: Failed to initialize MSFBO... Status: " << status << std::endl;

    // Also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    m_Texture = std::make_unique<Texture2D>(m_Width, m_Height, NULL);
    m_Texture->SetData(nullptr, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_Texture->SetFilterMode(GL_LINEAR);
    m_Texture->SetWrapMode(GL_REPEAT);
    m_Texture->Unbind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture->GetID(), 0); // attach texture to framebuffer as its color attachment
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSING: Failed to initialize FBO... Status: " << status << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    assert(m_Shader && m_Texture);

    // Initialize render data and uniforms
    Initialize();
    m_Shader->Use();
    m_Shader->SetInteger("u_RenderTarget", 0);
    constexpr float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right    
    };

    // TODO: Could be additional abilities in "Renderer/Shader.h"
    glUniform2fv(glGetUniformLocation(m_Shader->GetID(), "u_TextureOffsets"), 9, reinterpret_cast<float*>(offsets));
    int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(m_Shader->GetID(), "u_EdgeKernel"), 9, edge_kernel);
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(m_Shader->GetID(), "u_BlurKernel"), 9, blur_kernel);
}

PostProcess::~PostProcess()
{
    glDeleteRenderbuffers(1, &m_RBO);
    glDeleteFramebuffers(1, &m_MSFBO);
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void PostProcess::BeginFrame() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcess::EndFrame() const
{
    // Now resolve multi-sampled color-buffer into intermediate FBO to store to texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
    glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcess::Render(float deltaTime) const
{
    assert(m_Shader && m_Texture);

    // Set uniforms/options
    m_Shader->Use();
    m_Shader->SetFloat("u_Time", deltaTime);
    m_Shader->SetInteger("u_Mode", m_Mode);
    m_Shader->SetInteger("u_Confuse", m_Confuse);
    m_Shader->SetInteger("u_Chaos", m_Chaos);
    m_Shader->SetInteger("u_Shake", m_Shake);

    // Render textured quad
    m_Texture->Bind();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcess::Initialize()
{
    // Setup VAO / VBO
    unsigned int VBO;
    float vertices[] = {
        // positions        // tex coords
        -1.0f, -1.0f,       0.0f, 0.0f,
         1.0f,  1.0f,       1.0f, 1.0f,
        -1.0f,  1.0f,       0.0f, 1.0f,

        -1.0f, -1.0f,       0.0f, 0.0f,
         1.0f, -1.0f,       1.0f, 0.0f,
         1.0f,  1.0f,       1.0f, 1.0f
    };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
