#include "WindowManager.h"
#include "InputManager.h"

#include <iostream>

namespace GfxEngine3D
{
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputManager& inputManager = InputManager::GetInstance();
		InputManager::Key inputKey = InputManager::Key_None;
		switch (key) {
		case GLFW_KEY_ESCAPE:
			inputKey = InputManager::Key_Escape;
			break;
		case GLFW_KEY_W:
			inputKey = InputManager::Key_W;
			break;
		case GLFW_KEY_A:
			inputKey = InputManager::Key_A;
			break;
		case GLFW_KEY_S:
			inputKey = InputManager::Key_S;
			break;
		case GLFW_KEY_D:
			inputKey = InputManager::Key_D;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			inputKey = InputManager::Key_LShift;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			inputKey = InputManager::Key_LCtrl;
			break;
		case GLFW_KEY_SPACE:
			inputKey = InputManager::Key_Space;
			break;
		default:
			return;	// ignore other keys
		}
		if (action == GLFW_PRESS) {
			inputManager.OnKeyPressed(inputKey);
		}
		else if (action == GLFW_RELEASE) {
			inputManager.OnKeyReleased(inputKey);
		}
	}

	static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		InputManager& inputManager = InputManager::GetInstance();
		inputManager.OnMousePositionChanged(static_cast<float>(xpos), static_cast<float>(ypos));
	}

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		InputManager& inputManager = InputManager::GetInstance();
		InputManager::MouseButton inputBtn = InputManager::Mouse_None;
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			inputBtn = InputManager::Mouse_Left;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			inputBtn = InputManager::Mouse_Right;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			inputBtn = InputManager::Mouse_Middle;
			break;
		default:
			break; // ignore other buttons
		}
		if (action == GLFW_PRESS) {
			inputManager.OnMouseButtonPressed(inputBtn);
		}
		else if (action == GLFW_RELEASE) {
			inputManager.OnMouseButtonReleased(inputBtn);
		}
	}

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		InputManager& inputManager = InputManager::GetInstance();
		inputManager.OnMouseScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
	}

	WindowManager::WindowManager()
		: m_window(nullptr), m_initialized(false), m_width(0), m_height(0)
	{
	}

	WindowManager::~WindowManager()
	{
		Cleanup();
	}

	WindowManager& WindowManager::GetInstance()
	{
		static WindowManager instance;
		return instance;
	}

	void WindowManager::Init(int width, int height, const char* title)
	{
		if (m_initialized) {
			std::cerr << "WindowManager is already initialized" << std::endl;
			return;
		}

		// init GLFW
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// create window
		m_window = glfwCreateWindow(width, height, "MainWindow", nullptr, nullptr);
		if (m_window == nullptr) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(-1);
		}

		// set current context
		glfwMakeContextCurrent(m_window);

		// init GLAD
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			exit(-1);
		}

		// set viewport
		glViewport(0, 0, width, height);

		// register callbacks
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
		glfwSetKeyCallback(m_window, key_callback);
		glfwSetCursorPosCallback(m_window, mouse_position_callback);
		glfwSetMouseButtonCallback(m_window, mouse_button_callback);
		glfwSetScrollCallback(m_window, scroll_callback);

		m_width = width;
		m_height = height;
		m_initialized = true;
	}

	void WindowManager::ClearScreen() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void WindowManager::ProcessEvents()
	{
		glfwPollEvents();
		glfwGetWindowSize(m_window, &m_width, &m_height);
	}

	void WindowManager::Update() const
	{
		glfwSwapBuffers(m_window);
	}

	bool WindowManager::WindowShouldClose() const
	{
		return glfwWindowShouldClose(m_window) != 0;
	}

	void WindowManager::Cleanup()
	{
		if (m_initialized) {
			glfwDestroyWindow(m_window);
			glfwTerminate();
			m_initialized = false;
		}
	}

	void WindowManager::SetCursorEnabled(bool enabled) const
	{
		glfwSetInputMode(m_window, GLFW_CURSOR,
			enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	void WindowManager::SetClearColor(float r, float g, float b, float a) const
	{
		glClearColor(r, g, b, a);
	}

	void WindowManager::SetWindowShouldClose() const
	{
		glfwSetWindowShouldClose(m_window, true);
	}
}