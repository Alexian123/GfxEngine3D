#include "WindowManager.h"

#include <iostream>
#include "InputManager.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

WindowManager::WindowManager() : m_window(nullptr), m_initialized(false)
{
}

WindowManager& WindowManager::getInstance() {
	static WindowManager instance;
	return instance;
}

void WindowManager::init(int width, int height, const char* title)
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

	// register resize callback
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	m_initialized = true;
}

void WindowManager::update() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

bool WindowManager::windowShouldClose() const
{
	return glfwWindowShouldClose(m_window) != 0 ? true : false;
}

void WindowManager::cleanup() const
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}