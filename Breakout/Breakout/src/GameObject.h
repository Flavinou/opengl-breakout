#pragma once

#include "Renderer/Texture2D.h"
#include "Renderer/SpriteRenderer.h"

#include <glm/glm.hpp>

#include <memory>

class GameObject
{
public:
	GameObject();
	GameObject(const glm::vec2& position, const glm::vec2& size,
		const std::shared_ptr<Texture2D>& sprite, float rotation = 0.0f, 
		const glm::vec4& color = glm::vec4(1.0f), const glm::vec2& velocity = glm::vec2(0.0f, 0.0f));
	virtual ~GameObject() = default;

	virtual void Draw(const std::unique_ptr<SpriteRenderer>& renderer) const;

	glm::vec2& GetPosition() { return m_Position; }
	void SetPosition(const glm::vec2& position) { m_Position = position; }

	glm::vec2& GetSize() { return m_Size; }
	void SetSize(const glm::vec2& size) { m_Size = size; }

	glm::vec2& GetVelocity() { return m_Velocity; }
	void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	glm::vec4& GetColor() { return m_Color; }
	void SetColor(const glm::vec4& color) { m_Color = color; }

	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) { m_Rotation = rotation; }

	bool IsSolid() const { return m_IsSolid; }
	void SetSolid(bool isSolid) { m_IsSolid = isSolid; }
	bool IsDestroyed() const { return m_Destroyed; }
	void SetDestroyed(bool destroyed) { m_Destroyed = destroyed; }

	const std::shared_ptr<Texture2D>& GetSprite() { return m_Sprite; }
protected:
	// Object state
	glm::vec2 m_Position, m_Size, m_Velocity;
	glm::vec4 m_Color;     // Color of the object (RGB)
	float m_Rotation;      // Rotation angle in degrees
	bool m_IsSolid;      // Whether the object is solid (collidable)
	bool m_Destroyed;    // Whether the object is destroyed (for cleanup)

	// Render state
	std::shared_ptr<Texture2D> m_Sprite; // Texture of the object
};

