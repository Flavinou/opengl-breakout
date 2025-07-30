#pragma once

#include "Shader.h"
#include "Texture2D.h"

#include <memory>

class SpriteRenderer
{
public:
	SpriteRenderer(const std::shared_ptr<Shader>& shader);
	SpriteRenderer(const SpriteRenderer& other) = delete;
	~SpriteRenderer();

	void DrawSprite(const std::shared_ptr<Texture2D>& texture, 
		const glm::vec2& position, 
		const glm::vec2& size, 
		float rotation = 0.0f, 
		const glm::vec4& color = glm::vec4(1.0f));
private:
	void Initialize();
private:
	unsigned int m_QuadVAO, m_QuadVBO;
	std::shared_ptr<Shader> m_Shader;
};

