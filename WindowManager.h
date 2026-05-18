#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;

	static WindowManager& getInstance();

	void init(int width, int height, const char* title);
	void update() const;
	bool windowShouldClose() const;
	void cleanup() const;

	friend class InputManager;

protected:
	WindowManager();

private:
public:
	GLFWwindow* m_window;
	bool m_initialized;
};

