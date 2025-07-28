#pragma once

#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
    ~Shader() = default;

    void Use() const;

    void SetFloat(const char* name, float value) const;
    void SetInteger(const char* name, int value) const;
    void SetBool(const char* name, bool value) const;
    void SetVector2f(const char* name, float x, float y) const;
    void SetVector2f(const char* name, const glm::vec2& value) const;
    void SetVector3f(const char* name, float x, float y, float z) const;
    void SetVector3f(const char* name, const glm::vec3& value) const;
    void SetVector4f(const char* name, float x, float y, float z, float w) const;
    void SetVector4f(const char* name, const glm::vec4& value) const;
    void SetMatrix4(const char* name, const glm::mat4& matrix) const;

    const unsigned int& GetID() const { return m_ID; }
private:
    void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
    static void CheckCompileErrors(unsigned int id, const char* type);
private:
    unsigned int m_ID;
};