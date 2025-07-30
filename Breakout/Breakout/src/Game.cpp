#include "Game.h"

#include "ResourceManager.h"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>

Game::Game(const int width, const int height)
    : m_Width(width), m_Height(height), m_State(GameState::ACTIVE)
{
    Initialize();
}

Game::~Game()
{
}

void Game::Initialize()
{
    // Load shaders
	auto& resourceManager = ResourceManager::Instance();
    auto spriteShader = resourceManager.LoadShader("sprite", "resources/shaders/SpriteVertex.glsl", "resources/shaders/SpriteFragment.glsl");

    // Load textures
	auto texture = resourceManager.LoadTexture("face", "resources/textures/awesomeface.png", true);

    // Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
    spriteShader->Use();
	spriteShader->SetInteger("u_Texture", 0);
    spriteShader->SetMatrix4("u_Projection", projection);

    // Create sprite renderer
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);

    // Print initialization message
	std::cout << "Game initialized with resolution: " << m_Width << "x" << m_Height << '\n';
}

void Game::ProcessInput(float deltaTime)
{
    // TODO
}

void Game::Update(float deltaTime)
{
    // TODO
}

void Game::Render()
{
    m_SpriteRenderer->DrawSprite(
        ResourceManager::Instance().GetTexture("face"), 
        glm::vec2(200.0f, 200.0f), 
        glm::vec2(300.0f, 400.0f), 
        45.0f, 
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
    );
}
