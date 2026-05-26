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

	ShaderProgram entityShader("./Res/Shaders/entity_shader.vert", "./Res/Shaders/entity_shader.frag");
	ShaderProgram lightSourceShader("./Res/Shaders/entity_shader.vert", "./Res/Shaders/light_source_shader.frag");

	MeshLoader& meshLoader = MeshLoader::GetInstance();
	std::shared_ptr<Mesh> cubeMesh = meshLoader.LoadCube("cube", Mesh::Position | Mesh::TexCoord | Mesh::Normal);

	TextureLoader& textureLoader = TextureLoader::GetInstance();
	std::shared_ptr<Texture> crateDiffuse = textureLoader.LoadTexture("crate", "./Res/Textures/crate.png");
	std::shared_ptr<Texture> crateSpecular = textureLoader.LoadTexture("crate_specular", "./Res/Textures/crate_specular.png");

	std::shared_ptr<Material> crateMaterial = std::make_shared<Material>(
		crateDiffuse,	// diffuse
		crateSpecular,	// specular
		32.0f			// shininess
	);

	Entity crate(cubeMesh, crateMaterial);

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
	lights[0]->SetMesh(cubeMesh);
	lights[0]->SetScale(0.1f);
	lights[1]->SetMesh(cubeMesh);
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
		entityShader.Bind();

		entityShader.SetUniform("uModel", crate.GetModelMatrix());
		entityShader.SetUniform("uView", camera.GetViewMatrix());
		entityShader.SetUniform("uProjection", camera.GetProjectionMatrix());
		entityShader.SetUniform("uNormal", crate.GetNormalMatrix());
		entityShader.SetUniform("uViewPos", camera.GetPosition());
		entityShader.SetUniform("uMaterial.diffuse", 0);
		entityShader.SetUniform("uMaterial.specular", 1);
		entityShader.SetUniform("uMaterial.shininess", crate.GetMaterial()->GetShininess());
		entityShader.SetUniform("uNumLights", static_cast<int>(lights.size()));

		for (size_t i = 0; i < lights.size(); i++) {
			std::string lightUniformName = "uLights[" + std::to_string(i) + "]";
			entityShader.SetUniform(lightUniformName + ".position", lights[i]->GetPosition());
			entityShader.SetUniform(lightUniformName + ".ambient", lights[i]->GetAmbient());
			entityShader.SetUniform(lightUniformName + ".diffuse", lights[i]->GetDiffuse());
			entityShader.SetUniform(lightUniformName + ".specular", lights[i]->GetSpecular());
		}

		crate.GetMaterial()->GetDiffuse()->Bind(0);
		crate.GetMaterial()->GetSpecular()->Bind(1);

		crate.GetMesh()->Bind();
		crate.GetMesh()->Draw();
		crate.GetMesh()->Unbind();

		entityShader.Unbind();

		// render light source cubes
		lightSourceShader.Bind();
		lightSourceShader.SetUniform("uView", camera.GetViewMatrix());
		lightSourceShader.SetUniform("uProjection", camera.GetProjectionMatrix());
		for (const auto& light : lights) {
			auto mesh = light->GetMesh();
			if (mesh == nullptr) {
				continue;
			}
			lightSourceShader.SetUniform("uModel", light->GetModelMatrix());
			lightSourceShader.SetUniform("uColor", light->GetSpecular());
			light->GetMesh()->Bind();
			light->GetMesh()->Draw();
			light->GetMesh()->Unbind();
		}
		lightSourceShader.Unbind();

		// update window
		windowManager.Update();
	}

	// terminate
	windowManager.Cleanup();
	return 0;
}