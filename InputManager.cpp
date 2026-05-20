#include "InputManager.h"

InputManager::InputManager() 
	: 
	m_keyPressedMask(Key_None),
	m_mouseBtnPressedMask(Mouse_None),
	m_mouseX(0.0), m_mouseY(0.0)
{
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::processInput() const
{
	/*const WindowManager& windowManager = WindowManager::GetInstance();
	if (glfwGetKey(windowManager.m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(windowManager.m_window, true);
	}*/
}

void InputManager::OnKeyPressed(Key key)
{
	m_keyPressedMask |= key;
}

void InputManager::OnKeyReleased(Key key)
{
	m_keyPressedMask &= ~key;
}

void InputManager::OnMousePositionChanged(float x, float y)
{
	m_mouseX = x;
	m_mouseY = y;
}

void InputManager::OnMouseButtonPressed(MouseButton button)
{
	m_mouseBtnPressedMask |= button;
}

void InputManager::OnMouseButtonReleased(MouseButton button)
{
	m_mouseBtnPressedMask &= ~button;
}