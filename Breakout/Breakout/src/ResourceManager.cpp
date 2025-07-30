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
    auto shader = LoadShaderFromFile(vertexPath, fragmentPath, geometryPath);
    m_Shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)  
{  
    auto it = m_Shaders.find(name);  
    if (it != m_Shaders.end())  
    {  
        return it->second;  
    }

	// If shader not found, log an error message
	std::cerr << "[ERROR] Shader: Shader '" << name << "' not found.\n";

    return nullptr;  
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& name, const char* filePath, bool useAlphaChannel)
{
	auto texture = LoadTexture2DFromFile(filePath, useAlphaChannel);
    m_Textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture2D> ResourceManager::GetTexture(const std::string& name)
{
    auto it = m_Textures.find(name);
    if (it != m_Textures.end())
    {
        return it->second;
    }

    // If shader not found, log an error message
    std::cerr << "[ERROR] Texture2D: Texture '" << name << "' not found.\n";

    return nullptr;
}

void ResourceManager::Clear() const
{
    // Delete shaders
    for (auto& it : m_Shaders)
    {
        glDeleteProgram(it.second->GetID());
    }

    // Delete textures
    for (auto& it : m_Textures)
    {
        glDeleteTextures(1, &it.second->GetID());
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
    return std::make_shared<Shader>(vShaderCode, fShaderCode, geometryPath != nullptr ? gShaderCode : nullptr);
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture2DFromFile(const char* filePath, bool useAlphaChannel)
{
    // Load image
    int width, height, nrChannels;
    
    // Coordinate system is inverted by design, this is not required
    // stbi_set_flip_vertically_on_load(true);

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
