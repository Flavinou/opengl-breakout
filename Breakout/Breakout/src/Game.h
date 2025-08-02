#pragma once

#include "GameLevel.h"
#include "Renderer/SpriteRenderer.h"

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
	const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f); // Initial player paddle size
	const float PLAYER_VELOCITY = 500.0f; // Initial player paddle speed

    Game(int width, int height);
    Game(const Game& other) = delete;
    ~Game();

    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();

    GameState GetState() const { return m_State; }
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
};
