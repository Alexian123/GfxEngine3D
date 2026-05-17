#include <iostream>

#include "WindowManager.h"
#include "InputManager.h"
#include "ShaderProgram.h"
#include "Mesh.h"

int main() {
	WindowManager& windowManager = WindowManager::getInstance();
	windowManager.init(800, 600, "MainWindow");

	InputManager& inputManager = InputManager::getInstance();

	ShaderProgram shaderProgram("./Res/Shaders/shader.vert", "./Res/Shaders/shader.frag");

	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	Mesh rect(
		std::vector<float>(std::begin(vertices), std::end(vertices)), 
		std::vector<unsigned int>(std::begin(indices), std::end(indices)),
		true
	);

	float vertices2[] = {
		// positions
		 // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
	};
	
	Mesh triangle(
		std::vector<float>(std::begin(vertices2), std::end(vertices2)),
		true
	);

	// render loop
	while (!windowManager.windowShouldClose()) {
		// process input
		inputManager.processInput();

		// render frame
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Bind();

		shaderProgram.SetUniform("uBrightness", 0.4f);

		rect.Bind();
		rect.Draw();
		rect.Unbind();

		triangle.Bind();
		triangle.Draw();
		triangle.Unbind();

		shaderProgram.Unbind();

		// update window
		windowManager.update();
	}

	// terminate
	windowManager.cleanup();
	return 0;
}