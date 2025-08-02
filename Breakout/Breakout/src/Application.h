#pragma once

#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Application
{
public:
	Application(int width, int height, const char* title);
	Application(const Application& other) = delete;
	~Application();

	void Run();

	inline static Application& Get() { return *s_Instance; }
	inline GLFWwindow* GetWindow() const { return m_Window; }
	const bool* GetKeys() const { return m_Keys; }
private:
	void OnKeyPressed(int key, int scancode, int action, int mode);
	void OnWindowResize(int width, int height);
private:
	GLFWwindow* m_Window = nullptr;
	Game* m_Game = nullptr;

	int m_Width, m_Height;
	std::string m_Title;
	bool m_Keys[1024];
	float m_LastFrameTime = 0.0f;
private:
	static Application* s_Instance;
	friend int main(int argc, char** argv);
};

