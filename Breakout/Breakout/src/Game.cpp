#include "Game.h"

#include "Application.h"
#include "Physics/Physics2D.h"
#include "ResourceManager.h"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>

Game::Game(const int width, const int height)
    : m_Width(width), m_Height(height), m_State(GameState::ACTIVE)
{
    Initialize();
}

void Game::Initialize()
{
    // Load shaders
	auto& resourceManager = ResourceManager::Instance();
    const auto& spriteShader = resourceManager.LoadShader("sprite", "resources/shaders/SpriteVertex.glsl", "resources/shaders/SpriteFragment.glsl");
    const auto& particleShader = resourceManager.LoadShader("particle", "resources/shaders/ParticleVertex.glsl", "resources/shaders/ParticleFragment.glsl");
    const auto& postProcessingShader = resourceManager.LoadShader("postprocess", "resources/shaders/PostProcessVertex.glsl", "resources/shaders/PostProcessFragment.glsl");

    // Load textures
	resourceManager.LoadTexture("background", "resources/textures/background.jpg", false);
    resourceManager.LoadTexture("brick", "resources/textures/brick.png", false);
    resourceManager.LoadTexture("brick_solid", "resources/textures/brick_solid.png", false);
    const auto& ballTexture = resourceManager.LoadTexture("face", "resources/textures/awesomeface.png", true);
    const auto& paddleTexture = resourceManager.LoadTexture("paddle", "resources/textures/paddle.png", true);
    const auto& particleTexture = resourceManager.LoadTexture("particle", "resources/textures/particle.png", true);

    // Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
    spriteShader->Use();
	spriteShader->SetInteger("u_Texture", 0);
    spriteShader->SetMatrix4("u_Projection", projection);

    particleShader->Use();
    particleShader->SetMatrix4("u_Projection", projection);

    // Create sprite renderer
    m_SpriteRenderer = std::make_unique<SpriteRenderer>(spriteShader);

    // Print initialization message
	std::cout << "Game initialized with resolution: " << m_Width << "x" << m_Height << '\n';

	// Load levels
	m_Levels.reserve(4); // Reserve space for levels
    // m_Levels.push_back(std::make_unique<GameLevel>("resources/levels/solid.level", m_Width, m_Height / 2));
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

    // Create particles emitter, parent it to the Ball object
    m_ParticleEmitter = std::make_unique<ParticleEmitter>(particleShader, particleTexture, 500, m_Ball.get());

    // Create post-processing handler
    m_PostProcess = std::make_unique<PostProcess>(postProcessingShader, m_Width, m_Height);
}

void Game::ProcessInput(float deltaTime) const
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
    // Update objects positions
    m_Ball->Move(deltaTime, m_Width);

    // Check for collisions
    DoCollisions();

    // Update particles
    m_ParticleEmitter->Update(deltaTime);

    // Gradually decrease shake time
    if (m_ShakeTime > 0.0f)
    {
        m_ShakeTime -= deltaTime;
        if (m_ShakeTime <= 0.0f)
        {
            // Disable screen shake
            m_PostProcess->SetMode(PostProcessingFlags::None);
        }
    }

    // Loss condition
    DoCheckState();
}

void Game::Render(float deltaTime) const
{
    if (m_State == GameState::ACTIVE)
    {
        // Begin post-processing frame
        m_PostProcess->BeginFrame();

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

        // Render particles
        if (m_ParticleEmitter)
        {
            m_ParticleEmitter->Render();
        }

        // Render ball
        if (m_Ball)
        {
            m_Ball->Draw(m_SpriteRenderer);
        }

        // End post-processing frame, then render potential effects
        m_PostProcess->EndFrame();
        m_PostProcess->Render(deltaTime);
    }
}

void Game::DoCollisions()
{
    // No ball == no collision
    if (!m_Ball) return;

    // Check collision for each brick -> greedy
    for (unsigned int i = 0; i < m_Levels[m_CurrentLevelIndex]->Size(); ++i)
    {
        GameObject* box = m_Levels[m_CurrentLevelIndex]->GetBrick(i);
        if (box == nullptr) continue;
        if (box->IsDestroyed()) continue;

        // Collision detection / evaluation
        CollisionResult result = Physics2D::CheckCollisionCircleAABB(*m_Ball, *box);
        if (!std::get<0>(result)) continue; // No collision, go on

        // Collision detected !

        // Destroy block if possible
        if (!box->IsSolid())
        {
            box->Destroy();
        }
        else
        {
            // If block is solid, shake screen
            m_ShakeTime = 0.05f;
            m_PostProcess->SetMode(PostProcessingFlags::Shake);
        }

        // Collision resolution
        Direction dir = std::get<1>(result);
        glm::vec2 diff = std::get<2>(result);
        const auto& ballVelocity = m_Ball->GetVelocity();
        const auto& ballPosition = m_Ball->GetPosition();
        auto radius = m_Ball->GetRadius();
        if (dir == Direction::LEFT || dir == Direction::RIGHT) // Horizontal collision
        {
            // Reverse horizontal velocity
            m_Ball->SetVelocity(glm::vec2(-ballVelocity.x, ballVelocity.y));

            // Re-position ball by "penetration" amount
            float penetration = radius - std::abs(diff.x);
            glm::vec2 newPosition = glm::vec2(dir == Direction::LEFT ? ballPosition.x + penetration : ballPosition.x - penetration, ballPosition.y);
            m_Ball->SetPosition(newPosition);
        }
        else // Vertical collision
        {
            // Reverse vertical velocity
            m_Ball->SetVelocity(glm::vec2(ballVelocity.x, -ballVelocity.y));

            // Re-position ball by "penetration" amount
            float penetration = radius - std::abs(diff.y);
            glm::vec2 newPosition = glm::vec2(ballPosition.x, dir == Direction::UP ? ballPosition.y - penetration : ballPosition.y + penetration);
            m_Ball->SetPosition(newPosition);
        }
    }

    // Check collisions ball - player
    CollisionResult result = Physics2D::CheckCollisionCircleAABB(*m_Ball, *m_Player);
    if (m_Ball->IsStuck() || !std::get<0>(result)) return; // Ball stuck or no detection == no collision

    // If the ball collided with the player, check where it hit the board
    // then change velocity based on where it hit the board
    const auto& ballPos = m_Ball->GetPosition();
    auto radius = m_Ball->GetRadius();
    const auto& playerPos = m_Player->GetPosition();
    auto playerSize = m_Player->GetSize();
    float centerBoard = playerPos.x + playerSize.x / 2.0f;
    float distance = (ballPos.x + radius) - centerBoard;
    float amount = distance / (playerSize.x / 2.0f);

    constexpr float strength = 2.0f;
    glm::vec2 oldVelocity = m_Ball->GetVelocity();
    glm::vec2 newVelocity = glm::vec2(INITIAL_BALL_VELOCITY.x * amount * strength, -1.0f * abs(oldVelocity.y));
    m_Ball->SetVelocity(glm::normalize(newVelocity) * glm::length(oldVelocity));
}

void Game::DoCheckState()
{
    // Ball reached bottom edge -> Lose
    if (m_Ball->GetPosition().y >= m_Height)
    {
        End(GameState::LOST);
    }
}

void Game::End(GameState value) const
{
    // No state modification at the moment, reload the current level each time the ball is not caught by the player
    switch (value)
    {
    case GameState::LOST:
        ResetLevel();
        ResetPlayer();
        break;
    case GameState::WIN:
        break;
    default:
        break;
    }
}

void Game::ResetLevel() const
{
    if (!m_Levels[m_CurrentLevelIndex])
    {
        std::cout << "[ERROR] Level not found for index " << m_CurrentLevelIndex << " !" << std::endl;
    }

    // Reload current level
    GameLevel& level = *m_Levels[m_CurrentLevelIndex];
    level.Load(level.GetFileName(), m_Width, m_Height / 2);
}

void Game::ResetPlayer() const
{
    m_Player->SetSize(PLAYER_SIZE);
    m_Player->Reset(glm::vec2(m_Width / 2.0f - PLAYER_SIZE.x / 2.0f, m_Height - PLAYER_SIZE.y), glm::vec2(0.0f, 0.0f));
    m_Ball->Reset(m_Player->GetPosition() + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}
