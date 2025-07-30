#pragma once

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
};
