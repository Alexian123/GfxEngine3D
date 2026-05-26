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
	std::shared_ptr<Texture> crateDiffuse = textureLoader.LoadTexture("crate_diffuse", "./Res/Textures/crate.png");
	std::shared_ptr<Texture> crateSpecular = textureLoader.LoadTexture("crate_specular", "./Res/Textures/crate_specular.png");
	std::shared_ptr<Texture> crateEmission = textureLoader.LoadTexture("crate_emission", "./Res/Textures/matrix.jpg");

	std::shared_ptr<Material> crateMaterial = std::make_shared<Material>(
		crateDiffuse,	// diffuse
		crateSpecular,	// specular
		32.0f,			// shininess
		crateEmission	// emission
	);

	Entity crate(cubeMesh, crateMaterial);

	std::vector<std::shared_ptr<LightSource>> lights = {
		std::make_shared<LightSource>(
			glm::vec3(1.2f, 1.0f, 2.0f),
			glm::vec3(0.05f, 0.05f, 0.05f),
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		), 
		std::make_shared<LightSource>(
			glm::vec3(1.0f, 2.0f, 2.0f),
			glm::vec3(0.05f, 0.05f, 0.05f),
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		) 
	};
	lights[0]->SetMesh(cubeMesh);
	lights[0]->SetScale(0.1f);
	lights[1]->SetMesh(cubeMesh);
	lights[1]->SetScale(0.05f);

	FlyCamera camera(45.0f, windowManager.GetAspectRatio(), 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

	unsigned int numLightsMask = (lights.size() & 0xFFu) | (static_cast<unsigned long long>((1 & 0xFFu)) << 8);
		
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
		entityShader.SetUniform("uMaterial.emission", 2);
		entityShader.SetUniform("uMaterial.shininess", crate.GetMaterial()->GetShininess());

		// directional light
		entityShader.SetUniform("uDirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		entityShader.SetUniform("uDirLight.color.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		entityShader.SetUniform("uDirLight.color.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		entityShader.SetUniform("uDirLight.color.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point lights
		for (size_t i = 0; i < lights.size(); ++i) {
			std::string lightUniformName = "uPointLights[" + std::to_string(i) + "]";
			entityShader.SetUniform(lightUniformName + ".position", lights[i]->GetPosition());
			entityShader.SetUniform(lightUniformName + ".constant", 1.0f);
			entityShader.SetUniform(lightUniformName + ".linear", 0.09f);
			entityShader.SetUniform(lightUniformName + ".quadratic", 0.032f);
			entityShader.SetUniform(lightUniformName + ".color.ambient", lights[i]->GetAmbient());
			entityShader.SetUniform(lightUniformName + ".color.diffuse", lights[i]->GetDiffuse());
			entityShader.SetUniform(lightUniformName + ".color.specular", lights[i]->GetSpecular());
		}

		// spot lights
		for (size_t i = 0; i < 1; ++i) {
			std::string lightUniformName = "uSpotLights[" + std::to_string(i) + "]";
			entityShader.SetUniform(lightUniformName + ".direction", camera.GetFront());
			entityShader.SetUniform(lightUniformName + ".cutOff", glm::cos(glm::radians(12.5f)));
			entityShader.SetUniform(lightUniformName + ".outerCutOff", glm::cos(glm::radians(15.0f)));
			entityShader.SetUniform(lightUniformName + ".pl.position", camera.GetPosition());
			entityShader.SetUniform(lightUniformName + ".pl.constant", 1.0f);
			entityShader.SetUniform(lightUniformName + ".pl.linear", 0.09f);
			entityShader.SetUniform(lightUniformName + ".pl.quadratic", 0.032f);
			entityShader.SetUniform(lightUniformName + ".pl.color.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			entityShader.SetUniform(lightUniformName + ".pl.color.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			entityShader.SetUniform(lightUniformName + ".pl.color.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		entityShader.SetUniform("uNumLightsMask", numLightsMask);

		crate.GetMaterial()->GetDiffuse()->Bind(0);
		crate.GetMaterial()->GetSpecular()->Bind(1);
		//crate.GetMaterial()->GetEmission()->Bind(2);

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