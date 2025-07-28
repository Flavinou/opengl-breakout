#include "Game.h"

#include "Renderer/Renderer.h"
#include "ResourceManager.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

Game::Game(const int width, const int height, const char* title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    Initialize();
}

Game::~Game()
{
    ResourceManager::Instance().Clear();
    Renderer::Shutdown();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Game::Run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_DeltaTime = currentFrame - m_LastFrameTime;
        m_LastFrameTime = currentFrame;

        glfwPollEvents();

        // Process user input
        ProcessInput();

        // Update game state
        Update();

        // Render scene
        Render();

        Renderer::SetClearColor(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));
        Renderer::Clear();

        glfwSwapBuffers(m_Window);
    }
}

void Game::Initialize()
{
    // Initialize window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, true);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);

    // glad: Load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD." << '\n';
        return;
    }

    // Allows us to access the Application instance from GLFW callbacks
    glfwSetWindowUserPointer(m_Window, this);

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        const auto game = static_cast<Game*>(glfwGetWindowUserPointer(window));
        game->OnKeyPressed(key, scancode, action, mode);
    });
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
    {
        const auto game = static_cast<Game*>(glfwGetWindowUserPointer(window));
        game->OnWindowResize(width, height);
    });
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
    {
        const auto game = static_cast<Game*>(glfwGetWindowUserPointer(window));
        game->OnWindowResize(width, height);
    });

    // OpenGL Renderer setup
    Renderer::Initialize();
    Renderer::SetViewport(0, 0, m_Width, m_Height);
}

void Game::ProcessInput()
{
    // TODO
}

void Game::Update()
{
    // TODO
}

void Game::Render()
{
    // TODO
}

void Game::OnKeyPressed(int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            m_Keys[key] = true;
        else if (action == GLFW_RELEASE)
            m_Keys[key] = false;
    }
}

void Game::OnWindowResize(const int width, const int height)
{
    glViewport(0, 0, width, height);
}
