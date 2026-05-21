#include "InputManager.h"

InputManager::InputManager() 
{
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::BeginFrame()
{
	m_mouseDelta.x = 0.0f;
	m_mouseDelta.y = 0.0f;
	m_mouseScrollOffset.x = 0.0f;
	m_mouseScrollOffset.y = 0.0f;
}

void InputManager::OnKeyPressed(Key key)
{
	m_keyDownMask |= key;
}

void InputManager::OnKeyReleased(Key key)
{
	m_keyDownMask &= ~key;
}

void InputManager::OnMousePositionChanged(float x, float y)
{
	if (m_firstMouseUpdate)
	{
		m_mousePosition.x = x;
		m_mousePosition.y = y;
		m_firstMouseUpdate = false;
	}

	m_mouseDelta.x += x - m_mousePosition.x;
	m_mouseDelta.y += m_mousePosition.y - y; // reversed since y-coordinates go from bottom to top

	m_mousePosition.x = x;
	m_mousePosition.y = y;
}

void InputManager::OnMouseButtonPressed(MouseButton button)
{
	m_mouseBtnDownMask |= button;
}

void InputManager::OnMouseButtonReleased(MouseButton button)
{
	m_mouseBtnDownMask &= ~button;
}

void InputManager::OnMouseScroll(float xoffset, float yoffset)
{
	m_mouseScrollOffset.x += xoffset;
	m_mouseScrollOffset.y += yoffset;
}
