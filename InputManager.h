#pragma once

#include <cstdint>
#include <utility>

class InputManager
{
public:
	enum Key : std::uint64_t
    {
        Key_None    = 0,
        Key_Escape  = 1ULL << 0,
        Key_W       = 1ULL << 1,
        Key_A       = 1ULL << 2,
        Key_S       = 1ULL << 3,
        Key_D       = 1ULL << 4,
		Key_LShift	= 1ULL << 5,
		Key_LCtrl	= 1ULL << 6,
		Key_Space	= 1ULL << 7
    };

    enum MouseButton : std::uint8_t
    {
        Mouse_None   = 0,
        Mouse_Left   = 1 << 0,
        Mouse_Right  = 1 << 1,
        Mouse_Middle = 1 << 2
    };

	static InputManager& GetInstance();

	void OnKeyPressed(Key key);
	void OnKeyReleased(Key key);

	void OnMousePositionChanged(float x, float y);
	void OnMouseButtonPressed(MouseButton button);
	void OnMouseButtonReleased(MouseButton button);

	bool IsKeyPressed(Key key) const { return (m_keyPressedMask & key) != 0; }
	bool IsMouseButtonPressed(MouseButton button) const { return m_mouseBtnPressedMask & button; };

	auto GetMousePosition() const { return std::make_pair(m_mouseX, m_mouseY); }

protected:
	InputManager();
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

private:
	std::uint64_t m_keyPressedMask = 0;
	std::uint8_t m_mouseBtnPressedMask = 0;
	float m_mouseX = 0.0f, m_mouseY = 0.0f;
};