#pragma once

#include "GameLevel.h"
#include "Renderer/SpriteRenderer.h"
#include "GameObject.h"
#include "BallObject.h"

#include <memory>
#include <string>

// [CRITICAL] OpenGL function pointers must be included before GLFW !
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum GameState : uint8_t
{
    ACTIVE,
    MENU,
    WIN,
    LOST
};

class Game
{
public:
	static constexpr glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f); // Initial player paddle size
    static constexpr float PLAYER_VELOCITY = 500.0f; // Initial player paddle speed

    static constexpr float BALL_RADIUS = 12.5f;
    static constexpr glm::vec2 INITIAL_BALL_VELOCITY = glm::vec2(100.0f, -350.0f);

    Game(int width, int height);
    Game(const Game& other) = delete;
    ~Game() = default;

    GameState GetState() const { return m_State; }
    void SetState(GameState value) { m_State = value; }

    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render() const;
    void DoCollisions() const;
    void DoCheckState();

    void End(GameState value) const;

    void ResetLevel() const;
    void ResetPlayer() const;
    
private:
    void Initialize();
private:
	int m_Width, m_Height;
    GameState m_State;

	std::unique_ptr<SpriteRenderer> m_SpriteRenderer;

    // Gameplay state
	std::vector<std::unique_ptr<GameLevel>> m_Levels; // Array of game levels
	unsigned int m_CurrentLevelIndex; // Current level being played

    std::unique_ptr<GameObject> m_Player;
    std::unique_ptr<BallObject> m_Ball;
};
