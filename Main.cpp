#include <iostream>
#include <chrono>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "WindowManager.h"
#include "InputManager.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "FlyCamera.h"
#include "MeshLoader.h"
#include "TextureLoader.h"

int main() {
	WindowManager& windowManager = WindowManager::GetInstance();
	windowManager.Init(800, 600, "MainWindow");
	windowManager.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	windowManager.SetCursorEnabled(false);

	InputManager& inputManager = InputManager::GetInstance();

	ShaderProgram shaderProgram("./Res/Shaders/shader.vert", "./Res/Shaders/shader.frag");
	ShaderProgram lightSourceShaaderProgram("./Res/Shaders/shader.vert", "./Res/Shaders/light_source.frag");

	MeshLoader& meshLoader = MeshLoader::GetInstance();
	std::shared_ptr<Mesh> cube = meshLoader.LoadCube("cube1", Mesh::Position | Mesh::Color | Mesh::TexCoord | Mesh::Normal);
	std::shared_ptr<Mesh> lightSourceCube = meshLoader.LoadCube("light_source_cube", Mesh::Position);

	TextureLoader& textureLoader = TextureLoader::GetInstance();
	std::shared_ptr<Texture> brickTexture = textureLoader.LoadTexture("brick", "./Res/Textures/brick_texture.png");
	std::shared_ptr<Texture> patternTexture = textureLoader.LoadTexture("pattern", "./Res/Textures/pattern_texture.png");

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 45.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 cubeModelMatrix = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0))		// YXZ rotation
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1))
		* glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 cubeNormalMatrix = glm::mat4(glm::mat3(glm::transpose(glm::inverse(cubeModelMatrix))));

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::mat4 lightSourceModelMatrix = glm::mat4(1.0f);
	lightSourceModelMatrix = glm::translate(lightSourceModelMatrix, lightPos);
	lightSourceModelMatrix = glm::scale(lightSourceModelMatrix, glm::vec3(0.2f));

	FlyCamera camera(45.0f, windowManager.GetAspectRatio(), 0.1f, 100.0f);

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

		// render cube
		shaderProgram.Bind();

		shaderProgram.SetUniform("uBrightness", 1.0f);
		shaderProgram.SetUniform("uTexture", 0);
		shaderProgram.SetUniform("uTexture2", 1);
		shaderProgram.SetUniform("uModel", cubeModelMatrix);
		shaderProgram.SetUniform("uView", camera.GetViewMatrix());
		shaderProgram.SetUniform("uProjection", camera.GetProjectionMatrix());
		shaderProgram.SetUniform("uNormal", cubeNormalMatrix);
		shaderProgram.SetUniform("uLightColor", glm::vec3(1.0f));
		shaderProgram.SetUniform("uObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		shaderProgram.SetUniform("uLightPos", lightPos);
		shaderProgram.SetUniform("uViewPos", camera.GetPosition());

		brickTexture->Bind(0);
		patternTexture->Bind(1);

		cube->Bind();
		cube->Draw();
		cube->Unbind();

		shaderProgram.Unbind();

		// render light source cube
		lightSourceShaaderProgram.Bind();

		lightSourceShaaderProgram.SetUniform("uModel", lightSourceModelMatrix);
		lightSourceShaaderProgram.SetUniform("uView", camera.GetViewMatrix());
		lightSourceShaaderProgram.SetUniform("uProjection", camera.GetProjectionMatrix());

		lightSourceCube->Bind();
		lightSourceCube->Draw();
		lightSourceCube->Unbind();

		lightSourceShaaderProgram.Unbind();


		// update window
		windowManager.Update();
	}

	// terminate
	windowManager.Cleanup();
	return 0;
}