#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& name, const char* vertexPath, const char* fragmentPath,
                                                    const char* geometryPath /* = nullptr */)
{
    m_Shaders[name] = LoadShaderFromFile(vertexPath, fragmentPath, geometryPath);
    return m_Shaders[name].lock();
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)
{
    return m_Shaders[name].lock();
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& name, const char* filePath, bool useAlphaChannel)
{
    m_Textures[name] = LoadTexture2DFromFile(filePath, useAlphaChannel);
    return m_Textures[name].lock();
}

std::shared_ptr<Texture2D> ResourceManager::GetTexture(const std::string& name)
{
    return m_Textures[name].lock();
}

void ResourceManager::Clear() const
{
    // Delete shaders
    for (auto& it : m_Shaders)
    {
        glDeleteProgram(it.second.lock()->GetID());
    }

    // Delete textures
    for (auto& it : m_Textures)
    {
        glDeleteTextures(1, &it.second.lock()->GetID());
    }
}

std::shared_ptr<Shader> ResourceManager::LoadShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath /* = nullptr */)
{
    // Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vertexPath);
        std::ifstream fragmentShaderFile(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (geometryPath != nullptr)
        {
            std::ifstream geometryShaderFile(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception& e)
    {
        std::cout << "[ERROR] Shader: Failed to read shader files. Error: " << e.what() << '\n';
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // Create shader object from source code
    auto shader = std::make_shared<Shader>(vShaderCode, fShaderCode, geometryPath != nullptr ? gShaderCode : nullptr);
    return shader;
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture2DFromFile(const char* filePath, bool useAlphaChannel)
{
    // Load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);

    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

    // Create Texture2D object
    auto texture = std::make_shared<Texture2D>(width, height, nrChannels);
    texture->SetData(data, useAlphaChannel ? GL_RGBA : GL_RGB, useAlphaChannel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE);

    // Set default wrap / filter modes
    texture->SetFilterMode(GL_LINEAR);
    texture->SetWrapMode(GL_REPEAT);

    // Finally, free image data
    stbi_image_free(data);

    return texture;
}
