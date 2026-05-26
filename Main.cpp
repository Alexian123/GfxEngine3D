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
#include "LightSource.h"

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
	std::shared_ptr<Mesh> cube = meshLoader.LoadCube("cube1", Mesh::Position | Mesh::TexCoord | Mesh::Normal);
	std::shared_ptr<Mesh> lightSourceCube = meshLoader.LoadCube("light_source_cube", Mesh::Position);

	TextureLoader& textureLoader = TextureLoader::GetInstance();
	std::shared_ptr<Texture> brickTexture = textureLoader.LoadTexture("brick", "./Res/Textures/brick_texture.png");

	std::shared_ptr<Material> material1 = std::make_shared<Material>(
		brickTexture,					// diffuse
		glm::vec3(0.5f, 0.5f, 0.5f),	// specular
		32.0f							// shininess
	);

	Entity cubeEntity(cube, material1);

	std::vector<std::shared_ptr<LightSource>> lights = {
		std::make_shared<LightSource>(
			glm::vec3(1.2f, 1.0f, 2.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		), 
		std::make_shared<LightSource>(
			glm::vec3(1.0f, 2.0f, 2.0f),
			glm::vec3(0.2f, 0.2f, 0.2f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		) 
	};
	lights[0]->SetMesh(cube);
	lights[0]->SetScale(0.1f);
	lights[1]->SetMesh(cube);
	lights[1]->SetScale(0.05f);

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

		shaderProgram.SetUniform("uModel", cubeEntity.GetModelMatrix());
		shaderProgram.SetUniform("uView", camera.GetViewMatrix());
		shaderProgram.SetUniform("uProjection", camera.GetProjectionMatrix());
		shaderProgram.SetUniform("uNormal", cubeEntity.GetNormalMatrix());
		shaderProgram.SetUniform("uViewPos", camera.GetPosition());
		shaderProgram.SetUniform("uMaterial.diffuse", 0);
		shaderProgram.SetUniform("uMaterial.specular", cubeEntity.GetMaterial()->GetSpecular());
		shaderProgram.SetUniform("uMaterial.shininess", cubeEntity.GetMaterial()->GetShininess());
		shaderProgram.SetUniform("uNumLights", static_cast<int>(lights.size()));

		for (size_t i = 0; i < lights.size(); i++) {
			std::string lightUniformName = "uLights[" + std::to_string(i) + "]";
			shaderProgram.SetUniform(lightUniformName + ".position", lights[i]->GetPosition());
			shaderProgram.SetUniform(lightUniformName + ".ambient", lights[i]->GetAmbient());
			shaderProgram.SetUniform(lightUniformName + ".diffuse", lights[i]->GetDiffuse());
			shaderProgram.SetUniform(lightUniformName + ".specular", lights[i]->GetSpecular());
		}

		cubeEntity.GetMaterial()->GetDiffuse()->Bind(0);

		cubeEntity.GetMesh()->Bind();
		cubeEntity.GetMesh()->Draw();
		cubeEntity.GetMesh()->Unbind();

		shaderProgram.Unbind();

		// render light source cubes
		lightSourceShaaderProgram.Bind();
		lightSourceShaaderProgram.SetUniform("uView", camera.GetViewMatrix());
		lightSourceShaaderProgram.SetUniform("uProjection", camera.GetProjectionMatrix());
		for (const auto& light : lights) {
			auto mesh = light->GetMesh();
			if (mesh == nullptr) {
				continue;
			}
			lightSourceShaaderProgram.SetUniform("uModel", light->GetModelMatrix());
			lightSourceShaaderProgram.SetUniform("uColor", light->GetSpecular());
			light->GetMesh()->Bind();
			light->GetMesh()->Draw();
			light->GetMesh()->Unbind();
		}
		lightSourceShaaderProgram.Unbind();

		// update window
		windowManager.Update();
	}

	// terminate
	windowManager.Cleanup();
	return 0;
}