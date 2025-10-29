#include "Application.h"

#include "ResourceManager.h"
#include "Renderer/Renderer.h"

#include <iostream>

Application* Application::s_Instance = nullptr;

Application::Application(int width, int height, const char* title)
	: m_Width(width), m_Height(height), m_Title(title), m_Keys{false}
{
    s_Instance = this;

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
            const auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->OnKeyPressed(key, scancode, action, mode);
        });
    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            const auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->OnWindowResize(width, height);
        });
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            const auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            app->OnWindowResize(width, height);
        });

    // OpenGL Renderer setup
    Renderer::Initialize();
    Renderer::SetViewport(0, 0, m_Width, m_Height);

    // Initialize game
	m_Game = new Game(m_Width, m_Height);
}

Application::~Application()
{
    delete m_Game;

	if (m_Window)
	{
		glfwDestroyWindow(m_Window);
	}

    ResourceManager::Instance().Clear();
    Renderer::Shutdown();

	glfwTerminate();
}

void Application::Run()
{
    float deltaTime = 0.0f;
    while (!glfwWindowShouldClose(m_Window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - m_LastFrameTime;
        m_LastFrameTime = currentFrame;

        glfwPollEvents();

        // Process user input
        m_Game->ProcessInput(deltaTime);

        // Update game state
        m_Game->Update(deltaTime);

        Renderer::SetClearColor(glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));
        Renderer::Clear();

        // Render scene
        m_Game->Render(deltaTime);

        glfwSwapBuffers(m_Window);
    }
}

void Application::OnKeyPressed(int key, int scancode, int action, int mode)
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

void Application::OnWindowResize(const int width, const int height)
{
    glViewport(0, 0, width, height);
}