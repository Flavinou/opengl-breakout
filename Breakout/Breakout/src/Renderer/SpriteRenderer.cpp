#include "SpriteRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Shader>& shader)
	: m_Shader(shader)
{
	Initialize();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &m_QuadVAO);
	glDeleteBuffers(1, &m_QuadVBO);
}

void SpriteRenderer::DrawSprite(const std::shared_ptr<Texture2D>& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) const
{
	m_Shader->Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(size * 0.5f, 0.0f)); // Move origin to center of quad
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around center
	model = glm::translate(model, glm::vec3(-size * 0.5f, 0.0f)); // Move origin back to bottom left
	model = glm::scale(model, glm::vec3(size, 1.0f)); // Scale to size

	// Set shader uniforms
	m_Shader->SetVector4f("u_Color", color);
	m_Shader->SetMatrix4("u_Model", model);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	// Render quad
	glBindVertexArray(m_QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::Initialize()
{
	// Configure VAO/ VBO
	float vertices[] =
	{
		// positions         // texture coordinates
		0.0f, 1.0f, 0.0f, 1.0f, // top right
		1.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.0f, 0.0f, 0.0f, // top left

		0.0f, 1.0f, 0.0f, 1.0f, // top right
		1.0f, 1.0f, 1.0f, 1.0f, // bottom right
		1.0f, 0.0f, 1.0f, 0.0f	// bottom left
	};

	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &m_QuadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_QuadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}