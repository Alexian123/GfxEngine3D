#include <iostream>

#include "WindowManager.h"
#include "InputManager.h"
#include "ShaderProgram.h"
#include "Mesh.h"

int main() {
	WindowManager& windowManager = WindowManager::getInstance();
	windowManager.init(800, 600, "MainWindow");

	InputManager& inputManager = InputManager::getInstance();

	const char* vertexShaderSource = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";

	const char* fragmentShaderSource = 
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";

	ShaderProgram shaderProgram(vertexShaderSource, fragmentShaderSource);

	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	Mesh rect(std::vector<float>(std::begin(vertices), std::end(vertices)), 
		std::vector<unsigned int>(std::begin(indices), std::end(indices)));

	// render loop
	while (!windowManager.windowShouldClose()) {
		// process input
		inputManager.processInput();

		// render frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Bind();
		rect.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		rect.Unbind();
		shaderProgram.Unbind();

		// update window
		windowManager.update();
	}

	// terminate
	windowManager.cleanup();
	return 0;
}