#include "BallObject.h"

BallObject::BallObject()
    : m_Radius(12.5f)
{
}

BallObject::BallObject(const glm::vec2& position, const glm::vec2& velocity, const std::shared_ptr<Texture2D>& sprite, float radius)
    : GameObject(position, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, 0.0f, glm::vec4(1.0f), velocity), m_Radius(radius)
{
}

glm::vec2 BallObject::Move(float deltaTime, unsigned int viewportWidth)
{
    // Stuck, don't touch the ball position
    if (m_Stuck)
    {
        return m_Position;
    }

    // General case, write the ball's position over time
    m_Position += m_Velocity * deltaTime;

    if (m_Position.x <= 0.0f)
    {
        m_Velocity.x = -m_Velocity.x;
        m_Position.x = 0.0f;
    }

    if (m_Position.x + m_Size.x >= static_cast<float>(viewportWidth))
    {
        m_Velocity.x = -m_Velocity.x;
        m_Position.x = static_cast<float>(viewportWidth) - m_Size.x;
    }

    if (m_Position.y <= 0.0f)
    {
        m_Velocity.y = -m_Velocity.y;
        m_Position.y = 0.0f;
    }

    return m_Position;
}

void BallObject::Reset(const glm::vec2& position, const glm::vec2& velocity) /* override*/
{
    super::Reset(position, velocity);
    m_Stuck = true;
    m_Sticky = false;
    m_PassThrough = false;
}
