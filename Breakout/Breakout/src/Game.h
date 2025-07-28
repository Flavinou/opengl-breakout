#pragma once

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
    Game(int width, int height, const char* title);
    Game(const Game& other) = delete;
    ~Game();

    GameState GetState() const { return m_State; }
private:
    void Initialize();

    void Run();

    void ProcessInput();
    void Update();
    void Render();
private:
    void OnKeyPressed(int key, int scancode, int action, int mode);
    void OnWindowResize(int width, int height);
private:
    GameState m_State;
    GLFWwindow* m_Window = nullptr;
private:
    float m_DeltaTime = 0.0f;
    float m_LastFrameTime = 0.0f;

    bool m_Keys[1024];
    int m_Width, m_Height;
    std::string m_Title;
private:
    friend int main(int argc, char** argv);
};
