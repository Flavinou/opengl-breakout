#pragma once

#include "GameObject.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture2D.h"

#include <glm/glm.hpp>

#include <memory>

struct Particle
{
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float RemainingLifetime;
    float Scale;

    Particle();
    Particle(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color, float remainingLifetime, float scale);
    Particle(const Particle& other) = default;
    ~Particle() = default;
};

// Keeps track of a pool of particles during its lifetime
class ParticleEmitter
{
public:
    ParticleEmitter(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture,
                    unsigned int capacity, GameObject* parent = nullptr);
    ~ParticleEmitter();

    void Update(float deltaTime);
    void Render() const;

private:
    void Initialize();

    unsigned int GetAvailableParticleIndex();
    static void AwakeParticle(Particle& particle, const GameObject& object, const glm::vec2& offset);
private:
    unsigned int m_Capacity = 0;
    Particle** m_Particles = nullptr;
    unsigned int m_LastUsedIndex = 0;
    GameObject* m_Parent = nullptr;

    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture2D> m_Texture;
    unsigned int m_VAO = 0, m_VBO = 0;
};