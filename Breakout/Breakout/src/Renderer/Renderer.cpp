#include "Renderer.h"

#include <glad/glad.h>

void Renderer::Initialize()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);
}

void Renderer::Shutdown()
{
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
