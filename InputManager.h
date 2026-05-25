#pragma once

#include <cstdint>

#include <glm/vec2.hpp>

namespace GfxEngine3D
{
	class InputManager
	{
	public:
		enum Key : std::uint64_t
		{
			Key_None = 0,
			Key_Escape = 1ULL << 0,
			Key_W = 1ULL << 1,
			Key_A = 1ULL << 2,
			Key_S = 1ULL << 3,
			Key_D = 1ULL << 4,
			Key_LShift = 1ULL << 5,
			Key_LCtrl = 1ULL << 6,
			Key_Space = 1ULL << 7
		};

		enum MouseButton : std::uint8_t
		{
			Mouse_None = 0,
			Mouse_Left = 1 << 0,
			Mouse_Right = 1 << 1,
			Mouse_Middle = 1 << 2
		};

		static InputManager& GetInstance();

		void BeginFrame();

		void OnKeyPressed(Key key);
		void OnKeyReleased(Key key);

		void OnMousePositionChanged(float x, float y);
		void OnMouseButtonPressed(MouseButton button);
		void OnMouseButtonReleased(MouseButton button);
		void OnMouseScroll(float xoffset, float yoffset);

		bool IsKeyDown(Key key) const { return (m_keyDownMask & key) != 0; }
		bool IsMouseButtonDown(MouseButton button) const { return m_mouseBtnDownMask & button; };

		const glm::vec2& GetMousePosition() const { return m_mousePosition; };
		const glm::vec2& GetMouseDelta() const { return m_mouseDelta; }
		const glm::vec2& GetMouseScrollOffset() const { return m_mouseScrollOffset; }

	protected:
		InputManager();
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

	private:
		std::uint64_t m_keyDownMask = 0;
		std::uint8_t m_mouseBtnDownMask = 0;
		glm::vec2 m_mousePosition{ 0.0f, 0.0f };
		glm::vec2 m_mouseDelta{ 0.0f, 0.0f };
		glm::vec2 m_mouseScrollOffset{ 0.0f, 0.0f };
		bool m_firstMouseUpdate = true;
	};
}