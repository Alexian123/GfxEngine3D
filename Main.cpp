#include <iostream>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "WindowManager.h"
#include "InputManager.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"

int main() {
	WindowManager& windowManager = WindowManager::GetInstance();
	windowManager.Init(800, 600, "MainWindow");
	windowManager.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	windowManager.SetCursorEnabled(false);

	InputManager& inputManager = InputManager::GetInstance();

	ShaderProgram shaderProgram("./Res/Shaders/shader.vert", "./Res/Shaders/shader.frag");
	shaderProgram.SetDefaultVertexAttribute(1, 1.0f, 1.0f, 1.0f);	// default color white

	float vertices[] = { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f };

	Mesh rect(
		std::vector<float>(std::begin(vertices), std::end(vertices)),
		Mesh::TexCoord
	);

	Texture brickTexture("./Res/Textures/brick_texture.png");
	Texture patternTexture("./Res/Textures/pattern_texture.png");

	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 1.0f));
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::mat4_cast(rotation) * model;
	model = glm::scale(model, scale);

	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -89.0f;
	float pitch = 0.0f;

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

	// render loop
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;
	auto lastMousePos = inputManager.GetMousePosition();
	while (!windowManager.WindowShouldClose()) {
		// calculate delta time
		currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;
		float deltaTime = elapsed.count();
		lastTime = currentTime;

		// process events and input
		windowManager.ProcessEvents();
		auto currentMousePos = inputManager.GetMousePosition();

		if (inputManager.IsKeyPressed(InputManager::Key_Escape)) {
			windowManager.SetWindowShouldClose();
		}

		if (inputManager.IsMouseButtonPressed(InputManager::Mouse_Right)) {
			float mouseDeltaX = currentMousePos.first - lastMousePos.first;
			float mouseDeltaY = lastMousePos.second - currentMousePos.second;	// reversed since y-coordinates go from bottom to top

			const float sensitivity = 0.1f;
			mouseDeltaX *= sensitivity;
			mouseDeltaY *= sensitivity;

			yaw += mouseDeltaX;
			pitch += mouseDeltaY;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(direction);
		}

		const float cameraSpeed = 2.5f * deltaTime;
		if (inputManager.IsKeyPressed(InputManager::Key_W))
			cameraPos += cameraSpeed * cameraFront;
		if (inputManager.IsKeyPressed(InputManager::Key_S))
			cameraPos -= cameraSpeed * cameraFront;
		if (inputManager.IsKeyPressed(InputManager::Key_A))
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (inputManager.IsKeyPressed(InputManager::Key_D))
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (inputManager.IsKeyPressed(InputManager::Key_Space))
			cameraPos += cameraSpeed * cameraUp;
		if (inputManager.IsKeyPressed(InputManager::Key_LCtrl))
			cameraPos -= cameraSpeed * cameraUp;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// render frame
		windowManager.ClearScreen();
		shaderProgram.Bind();

		shaderProgram.SetUniform("uBrightness", 1.0f);
		shaderProgram.SetUniform("uTexture", 0);
		shaderProgram.SetUniform("uTexture2", 1);

		shaderProgram.SetUniform("uModel", model);
		shaderProgram.SetUniform("uView", view);
		shaderProgram.SetUniform("uProjection", projection);

		brickTexture.Bind(0);
		patternTexture.Bind(1);

		rect.Bind();
		rect.Draw();
		rect.Unbind();

		shaderProgram.Unbind();

		// update window
		windowManager.Update();

		lastMousePos = currentMousePos;
	}

	// terminate
	windowManager.Cleanup();
	return 0;
}