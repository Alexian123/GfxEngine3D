#include "InputManager.h"

InputManager::InputManager() 
{
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
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