#include "ParticleSystem.h"

#include <glad/glad.h>

Particle::Particle()
    : Position(0.0f), Velocity(0.0f), Color(1.0f), RemainingLifetime(0.0f), Scale(10.0f)
{
}

Particle::Particle(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color,
    const float remainingLifetime, const float scale)
    : Position(position), Velocity(velocity), Color(color), RemainingLifetime(remainingLifetime), Scale(scale)
{
}

ParticleEmitter::ParticleEmitter(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture2D>& texture, unsigned int capacity, GameObject* parent /* = nullptr*/)
    : m_Capacity(capacity), m_Parent(parent), m_Shader(shader), m_Texture(texture)
{
    m_Particles = new Particle*[m_Capacity];

    Initialize();
}

ParticleEmitter::~ParticleEmitter()
{
    delete[] m_Particles;
}

void ParticleEmitter::Update(float deltaTime)
{
    unsigned int nbNewParticles = 2;

    // Add new particles
    for (unsigned int i = 0; i < nbNewParticles; ++i)
    {
        unsigned int unusedParticleIndex = GetAvailableParticleIndex();
        AwakeParticle(*m_Particles[unusedParticleIndex], *m_Parent, m_Parent->GetSize() / 2.0f);
    }

    // Update all particles
    for (unsigned int i = 0; i < m_Capacity; ++i)
    {
        Particle& particle = *m_Particles[i];
        particle.RemainingLifetime -= deltaTime;

        if (particle.RemainingLifetime > 0.0f)
        {
            particle.Position -= particle.Velocity * deltaTime; // Move particle over time
            particle.Color.a -= deltaTime * 2.5f; // Fade color over time
        }
    }
}

void ParticleEmitter::Render() const
{
    // Additive blending to get a "glow" effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_Shader->Use();

    for (unsigned int i = 0; i < m_Capacity; ++i)
    {
        const Particle& particle = *m_Particles[i];
        if (particle.RemainingLifetime > 0.0f)
        {
            m_Shader->SetVector2f("u_Offset", particle.Position);
            m_Shader->SetVector4f("u_Color", particle.Color);
            m_Shader->SetFloat("u_Scale", particle.Scale);
            m_Texture->Bind();
            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int ParticleEmitter::GetAvailableParticleIndex()
{
    // Search from last used particle
    for (unsigned int i = m_LastUsedIndex; i < m_Capacity; ++i)
    {
        if (m_Particles[i]->RemainingLifetime <= 0.0f)
        {
            m_LastUsedIndex = i;
            return i;
        }
    }

    // Not found, do a linear search from the start
    for (unsigned int i = 0; i < m_LastUsedIndex; ++i)
    {
        if (m_Particles[i]->RemainingLifetime <= 0.0f)
        {
            m_LastUsedIndex = i;
            return i;
        }
    }

    // If all particles are alive, force the first one to be the last used
    m_LastUsedIndex = 0;
    return 0;
}

void ParticleEmitter::AwakeParticle(Particle& particle, const GameObject& object, const glm::vec2& offset)
{
    float random = ((static_cast<float>(rand() % 100)) - 50) / 10.0f;
    float randomColor = 0.5f + ((static_cast<float>(rand() % 100)) / 100.0f);
    particle.Position = object.GetPosition() + random + offset;
    particle.Color = glm::vec4(glm::vec3(randomColor), 1.0f);
    particle.RemainingLifetime = 1.0f;
    particle.Velocity = object.GetVelocity() * 0.1f;
}

void ParticleEmitter::Initialize()
{
    float quadVertices[] = 
    {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(nullptr));
    glBindVertexArray(0);

    // Allocate memory for particles
    for (unsigned int i = 0; i < m_Capacity; ++i)
    {
        m_Particles[i] = new Particle();
    }
}
