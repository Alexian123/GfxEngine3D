#include "InputManager.h"
#include "WindowManager.h"

InputManager::InputManager()
{
}

InputManager& InputManager::getInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::processInput() const
{
	const WindowManager& windowManager = WindowManager::getInstance();
	if (glfwGetKey(windowManager.m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(windowManager.m_window, true);
	}
}