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
#include "Material.h"
#include "Entity.h"

using namespace GfxEngine3D;

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

	std::shared_ptr<Material> material1 = std::make_shared<Material>(
		glm::vec3(1.0f, 0.5f, 0.31f), // ambient
		glm::vec3(1.0f, 0.5f, 0.31f), // diffuse
		glm::vec3(0.5f, 0.5f, 0.5f),  // specular
		32.0f						  // shininess
	);

	Entity cubeEntity(cube, brickTexture, material1);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::mat4 lightSourceModelMatrix = glm::mat4(1.0f);
	lightSourceModelMatrix = glm::translate(lightSourceModelMatrix, lightPos);
	lightSourceModelMatrix = glm::scale(lightSourceModelMatrix, glm::vec3(0.1f));

	glm::vec3 lightPos2(1.0f, 2.0f, 2.0f);
	glm::mat4 lightSourceModelMatrix2 = glm::mat4(1.0f);
	lightSourceModelMatrix2 = glm::translate(lightSourceModelMatrix2, lightPos2);
	lightSourceModelMatrix2 = glm::scale(lightSourceModelMatrix2, glm::vec3(0.05f));

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
		shaderProgram.SetUniform("uModel", cubeEntity.GetModelMatrix());
		shaderProgram.SetUniform("uView", camera.GetViewMatrix());
		shaderProgram.SetUniform("uProjection", camera.GetProjectionMatrix());
		shaderProgram.SetUniform("uNormal", cubeEntity.GetNormalMatrix());
		shaderProgram.SetUniform("uViewPos", camera.GetPosition());
		shaderProgram.SetUniform("uMaterial.ambient", cubeEntity.GetMaterial()->GetAmbient());
		shaderProgram.SetUniform("uMaterial.diffuse", cubeEntity.GetMaterial()->GetDiffuse());
		shaderProgram.SetUniform("uMaterial.specular", cubeEntity.GetMaterial()->GetSpecular());
		shaderProgram.SetUniform("uMaterial.shininess", cubeEntity.GetMaterial()->GetShininess());

		shaderProgram.SetUniform("uNumLights", 2);

		shaderProgram.SetUniform("uLights[0].position", lightPos);
		shaderProgram.SetUniform("uLights[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.SetUniform("uLights[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darkened
		shaderProgram.SetUniform("uLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

		shaderProgram.SetUniform("uLights[1].position", lightPos2);
		shaderProgram.SetUniform("uLights[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.SetUniform("uLights[1].diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darkened
		shaderProgram.SetUniform("uLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));

		cubeEntity.GetTexture()->Bind(0);

		cubeEntity.GetMesh()->Bind();
		cubeEntity.GetMesh()->Draw();
		cubeEntity.GetMesh()->Unbind();

		shaderProgram.Unbind();

		// render light source cube
		lightSourceShaaderProgram.Bind();

		lightSourceCube->Bind();

		lightSourceShaaderProgram.SetUniform("uModel", lightSourceModelMatrix);
		lightSourceShaaderProgram.SetUniform("uView", camera.GetViewMatrix());
		lightSourceShaaderProgram.SetUniform("uProjection", camera.GetProjectionMatrix());
		lightSourceCube->Draw();

		lightSourceShaaderProgram.SetUniform("uModel", lightSourceModelMatrix2);
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