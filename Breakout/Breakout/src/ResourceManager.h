#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Texture2D.h"

#include <memory>
#include <unordered_map>
#include <string>

class ResourceManager
{
public:
    static ResourceManager& Instance();

    std::shared_ptr<Shader> LoadShader(const std::string& name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    std::shared_ptr<Shader> GetShader(const std::string& name);

    std::shared_ptr<Texture2D> LoadTexture(const std::string& name, const char* filePath, bool useAlphaChannel);
    std::shared_ptr<Texture2D> GetTexture(const std::string& name);

    void Clear() const;
private:
    ResourceManager() = default;
    static std::shared_ptr<Shader> LoadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    static std::shared_ptr<Texture2D> LoadTexture2DFromFile(const char* filePath, bool useAlphaChannel);

    friend class Shader;
    friend class Texture2D;
private:
    std::unordered_map<std::string, std::weak_ptr<Shader>> m_Shaders;
    std::unordered_map<std::string, std::weak_ptr<Texture2D>> m_Textures;
};
