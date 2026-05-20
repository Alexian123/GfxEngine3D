#pragma once

#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	~WindowManager();

	static WindowManager& GetInstance();

	void Init(int width, int height, const char* title);
	void ClearScreen() const;
	void ProcessEvents();
	void Update() const;
	bool WindowShouldClose() const;
	void Cleanup();

	void SetCursorEnabled(bool enabled) const;
	void SetClearColor(float r, float g, float b, float a) const;
	void SetWindowShouldClose() const;

	auto GetWindowDimensions() const { return std::make_pair(m_width, m_height); }

protected:
	WindowManager();
	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;

private:
	GLFWwindow* m_window;
	bool m_initialized;
	int m_width, m_height;
};

