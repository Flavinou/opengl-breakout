#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

Shader::Shader(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    Compile(vertexSource, fragmentSource, geometrySource);
}

void Shader::Use() const
{
    glUseProgram(m_ID);
}

void Shader::SetFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name), value);
}

void Shader::SetInteger(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name), value);
}

void Shader::SetBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name), static_cast<int>(value));
}

void Shader::SetVector2f(const char* name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_ID, name), x, y);
}

void Shader::SetVector2f(const char* name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}

void Shader::SetVector3f(const char* name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_ID, name), x, y, z);
}

void Shader::SetVector3f(const char* name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}

void Shader::SetVector4f(const char* name, const float x, const float y, const float z, const float w) const
{
    glUniform4f(glGetUniformLocation(m_ID, name), x, y, z, w);
}

void Shader::SetVector4f(const char* name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_ID, name), 1, &value[0]);
}

void Shader::SetMatrix4(const char* name, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int geometryID = 0;

    // vertex Shader
    unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vertexSource, nullptr);
    glCompileShader(vertexID);
    CheckCompileErrors(vertexID, "VERTEX");

    // fragment Shader
    unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentID);
    CheckCompileErrors(fragmentID, "FRAGMENT");

    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr)
    {
        geometryID = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryID, 1, &geometrySource, nullptr);
        glCompileShader(geometryID);
        CheckCompileErrors(geometryID, "GEOMETRY");
    }

    // shader program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexID);
    glAttachShader(m_ID, fragmentID);

    if (geometrySource != nullptr)
        glAttachShader(m_ID, geometryID);

    glLinkProgram(m_ID);
    CheckCompileErrors(m_ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    if (geometrySource != nullptr)
        glDeleteShader(geometryID);
}

void Shader::CheckCompileErrors(const unsigned int id, const char* type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, nullptr, infoLog);
            std::cout << "| [ERROR] Shader: Compile-time error: Type: " << type << '\n'
                << infoLog << "\n -- --------------------------------------------------- -- "
                << '\n';
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, nullptr, infoLog);
            std::cout << "| [ERROR] Shader: Link-time error: Type: " << type << '\n'
                << infoLog << "\n -- --------------------------------------------------- -- "
                << '\n';
        }
    }
}
