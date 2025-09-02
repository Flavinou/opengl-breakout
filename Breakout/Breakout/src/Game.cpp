#include "Game.h"

#include "ResourceManager.h"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>
#include "Application.h"

Game::Game(const int width, const int height)
    : m_Width(width), m_Height(height), m_State(GameState::ACTIVE)
{
    Initialize();
}

void Game::Initialize()
{
    // Load shaders
	auto& resourceManager = ResourceManager::Instance();
    auto spriteShader = resourceManager.LoadShader("sprite", "resources/shaders/SpriteVertex.glsl", "resources/shaders/SpriteFragment.glsl");

    // Load textures
	resourceManager.LoadTexture("background", "resources/textures/background.jpg", false);
    resourceManager.LoadTexture("brick", "resources/textures/brick.png", false);
    resourceManager.LoadTexture("brick_solid", "resources/textures/brick_solid.png", false);
    auto ballTexture = resourceManager.LoadTexture("face", "resources/textures/awesomeface.png", true);
    auto paddleTexture = resourceManager.LoadTexture("paddle", "resources/textures/paddle.png", true);

    // Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
    spriteShader->Use();
	spriteShader->SetInteger("u_Texture", 0);
    spriteShader->SetMatrix4("u_Projection", projection);

    // Create sprite renderer
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);

    // Print initialization message
	std::cout << "Game initialized with resolution: " << m_Width << "x" << m_Height << '\n';

	// Load levels
	m_Levels.reserve(4); // Reserve space for levels
	m_Levels.push_back(std::make_unique<GameLevel>("resources/levels/one.level", m_Width, m_Height / 2));
	m_Levels.push_back(std::make_unique<GameLevel>("resources/levels/two.level", m_Width, m_Height / 2));
	m_Levels.push_back(std::make_unique<GameLevel>("resources/levels/three.level", m_Width, m_Height / 2));
	m_Levels.push_back(std::make_unique<GameLevel>("resources/levels/four.level", m_Width, m_Height / 2));
	m_CurrentLevelIndex = 0; // Start with the first level

    // Print loaded levels
	std::cout << "Loaded " << m_Levels.size() << " levels.\n";

	// Initialize player
	glm::vec2 playerPosition(m_Width / 2.0f - PLAYER_SIZE.x, m_Height - PLAYER_SIZE.y);
    m_Player = std::make_unique<GameObject>(playerPosition, PLAYER_SIZE, paddleTexture);

    // Initialize ball
    glm::vec2 ballPosition(playerPosition + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f));
    m_Ball = std::make_unique<BallObject>(ballPosition, INITIAL_BALL_VELOCITY, ballTexture, BALL_RADIUS);
}

void Game::ProcessInput(float deltaTime)
{
    if (m_State == GameState::ACTIVE)
    {
		float velocity = PLAYER_VELOCITY * deltaTime;
		auto keys = Application::Get().GetKeys();
		auto playerPos = m_Player->GetPosition();
		auto playerSize = m_Player->GetSize();
        auto ballPos = m_Ball->GetPosition();

        if (keys[GLFW_KEY_A])
        {
            if (playerPos.x >= 0.0f)
            {
                m_Player->SetPosition(glm::vec2(playerPos.x - velocity, playerPos.y));
                if (m_Ball->IsStuck())
                {
                    m_Ball->SetPosition(glm::vec2(ballPos.x - velocity, ballPos.y));
                }
            }
        }
        if (keys[GLFW_KEY_D])
        {
            if (playerPos.x <= m_Width - playerSize.x)
            {
                m_Player->SetPosition(glm::vec2(playerPos.x + velocity, playerPos.y));
                if (m_Ball->IsStuck())
                {
                    m_Ball->SetPosition(glm::vec2(ballPos.x + velocity, ballPos.y));
                }
            }
        }
        if (keys[GLFW_KEY_SPACE])
        {
            m_Ball->SetStuck(false);
        }
    }
}

void Game::Update(float deltaTime)
{
    m_Ball->Move(deltaTime, m_Width);
}

void Game::Render()
{
    if (m_State == GameState::ACTIVE)
    {
		// Render background
        auto backgroundTexture = ResourceManager::Instance().GetTexture("background");
        if (backgroundTexture)
        {
            m_SpriteRenderer->DrawSprite(backgroundTexture, glm::vec2(0.0f, 0.0f), glm::vec2(m_Width, m_Height));
        }

        // Render current level
        if (m_CurrentLevelIndex < m_Levels.size())
        {
            m_Levels[m_CurrentLevelIndex]->Draw(m_SpriteRenderer);
        }

		// Render player paddle
        if (m_Player)
        {
			m_Player->Draw(m_SpriteRenderer);
        }

        // Render ball
        if (m_Ball)
        {
            m_Ball->Draw(m_SpriteRenderer);
        }
    }
}
