#pragma once

#include <cstdint>
#include <utility>

class InputManager
{
public:
	enum Key
	{
		Key_None		= 0x0,
		Key_Escape		= 0x1,
		Key_W			= 0x2,
		Key_A			= 0x4,
		Key_S			= 0x8,
		Key_D			= 0xA,
	};

	enum MouseButton
	{
		Mouse_None		= 0x0,
		Mouse_Left		= 0x1,
		Mouse_Right		= 0x2,
		Mouse_Middle	= 0x3
	};

	static InputManager& GetInstance();

	void processInput() const;

	void OnKeyPressed(Key key);
	void OnKeyReleased(Key key);

	void OnMousePositionChanged(float x, float y);
	void OnMouseButtonPressed(MouseButton button);
	void OnMouseButtonReleased(MouseButton button);

	bool IsKeyPressed(Key key) const { return m_keyPressedMask & key; };
	bool IsMouseButtonPressed(MouseButton button) const { return m_mouseBtnPressedMask & button; };
	auto GetMousePosition() const { return std::make_pair(m_mouseX, m_mouseY); }

protected:
	InputManager();
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

private:
	std::uint64_t m_keyPressedMask;
	std::uint8_t m_mouseBtnPressedMask;
	float m_mouseX, m_mouseY;
};