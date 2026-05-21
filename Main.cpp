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
#include "FlyCamera.h"

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

	FlyCamera camera(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

	// render loop
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;
	while (!windowManager.WindowShouldClose()) {
		// calculate delta time
		currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;
		float deltaTime = elapsed.count();
		lastTime = currentTime;

		// process events and input
		inputManager.BeginFrame();
		windowManager.ProcessEvents();	

		if (inputManager.IsKeyDown(InputManager::Key_Escape)) {
			windowManager.SetWindowShouldClose();
		}

		auto scrollOffset = inputManager.GetMouseScrollOffset();
		if (scrollOffset.y != 0.0f) {
			camera.Zoom(-scrollOffset.y);
		}

		if (inputManager.IsMouseButtonDown(InputManager::Mouse_Right)) {
			const auto& mouseDelta = inputManager.GetMouseDelta();
			const float sensitivity = 0.1f;
			camera.RotateYaw(mouseDelta.x * sensitivity);
			camera.RotatePitch(mouseDelta.y * sensitivity);
		}

		const float cameraSpeed = 2.5f * deltaTime;
		if (inputManager.IsKeyDown(InputManager::Key_W))
			camera.MoveZ(cameraSpeed);
		if (inputManager.IsKeyDown(InputManager::Key_S))
			camera.MoveZ(-cameraSpeed);
		if (inputManager.IsKeyDown(InputManager::Key_A))
			camera.MoveX(-cameraSpeed);
		if (inputManager.IsKeyDown(InputManager::Key_D))
			camera.MoveX(cameraSpeed);
		if (inputManager.IsKeyDown(InputManager::Key_Space))
			camera.MoveY(cameraSpeed);
		if (inputManager.IsKeyDown(InputManager::Key_LCtrl))
			camera.MoveY(-cameraSpeed);

		// render frame
		windowManager.ClearScreen();
		shaderProgram.Bind();

		shaderProgram.SetUniform("uBrightness", 1.0f);
		shaderProgram.SetUniform("uTexture", 0);
		shaderProgram.SetUniform("uTexture2", 1);

		shaderProgram.SetUniform("uModel", model);
		shaderProgram.SetUniform("uView", camera.GetViewMatrix());
		shaderProgram.SetUniform("uProjection", camera.GetProjectionMatrix());

		brickTexture.Bind(0);
		patternTexture.Bind(1);

		rect.Bind();
		rect.Draw();
		rect.Unbind();

		shaderProgram.Unbind();

		// update window
		windowManager.Update();
	}

	// terminate
	windowManager.Cleanup();
	return 0;
}