#include "GameObject.h"
#include "GameLevel.h"

GameObject::GameObject()
	: m_Position(0.0f, 0.0f), m_Size(1.0f, 1.0f), m_Velocity(0.0f, 0.0f), 
	m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Rotation(0.0f), m_IsSolid(false), m_Destroyed(false)
{
}

GameObject::GameObject(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& sprite, float rotation, const glm::vec4& color, const glm::vec2& velocity)
	: m_Position(position), m_Size(size), 
	m_Sprite(sprite), m_Rotation(rotation), 
	m_Color(color), m_Velocity(velocity),
	m_IsSolid(false), m_Destroyed(false)
{
}

void GameObject::Draw(const std::unique_ptr<SpriteRenderer>& renderer) const
{
	renderer->DrawSprite(m_Sprite, m_Position, m_Size, m_Rotation, m_Color);
}

void GameObject::Reset(const glm::vec2& position, const glm::vec2& velocity)
{
	m_Position = position;
	m_Velocity = velocity;
}
