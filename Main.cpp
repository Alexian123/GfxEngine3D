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
#include "Lights.h"

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
	
	std::vector<DirLight> dirLights = {
		{
			.color = {
				.ambient = glm::vec3(0.05f, 0.05f, 0.05f),
				.diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
				.specular = glm::vec3(0.5f, 0.5f, 0.5f)
			},
			.direction = glm::vec3(-0.2f, -1.0f, -0.3f)
		}
	};

	std::vector<PointLight> pointLights = {
		{
			.color = {
				.ambient = glm::vec3(0.05f, 0.05f, 0.05f),
				.diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
				.specular = glm::vec3(1.0f, 1.0f, 1.0f)
			},
			.position = glm::vec3(1.2f, 1.0f, 2.0f),
			.constant = 1.0f,
			.linear = 0.09f,
			.quadratic = 0.032f
		},
		{
			.color = {
				.ambient = glm::vec3(0.05f, 0.05f, 0.05f),
				.diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
				.specular = glm::vec3(1.0f, 1.0f, 1.0f)
			},
			.position = glm::vec3(1.0f, 2.0f, 2.0f),
			.constant = 1.0f,
			.linear = 0.09f,
			.quadratic = 0.032f
		}
	};

	std::vector<std::shared_ptr<Entity>> lightSourceEntities;
	for (const auto& pl : pointLights) {
		auto lightSource = std::make_shared<Entity>(cubeMesh);
		lightSource->SetScale(0.1f);
		lightSource->SetPosition(pl.position);
		lightSourceEntities.push_back(lightSource);
	}

	std::vector<SpotLight> spotLights = {
		{
			.pl = {
				.color = {
					.ambient = glm::vec3(0.0f, 0.0f, 0.0f),
					.diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
					.specular = glm::vec3(1.0f, 1.0f, 1.0f)
				},
				.position = glm::vec3(0.0f),
				.constant = 1.0f,
				.linear = 0.09f,
				.quadratic = 0.032f
			},
			.direction = glm::vec3(0.0f),
			.cutOff = glm::cos(glm::radians(12.5f)),
			.outerCutOff = glm::cos(glm::radians(15.0f)),
		}
	};

	FlyCamera camera(45.0f, windowManager.GetAspectRatio(), 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

	uint32_t numLightsMask = static_cast<uint32_t>(1)	// dir lights
		| ((static_cast<uint32_t>(pointLights.size()) & 0xFFu) << 8)	// point lights
		| (static_cast<uint32_t>(1) << 16); // spot lights
		
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

		numLightsMask &= 0xFFFF;
		if (inputManager.IsMouseButtonDown(InputManager::Mouse_Left)) {
			numLightsMask |= (static_cast<uint32_t>(1) << 16);
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

		// update spot light position and direction to simulate FPS flashlight
		spotLights[0].pl.position = camera.GetPosition();
		spotLights[0].direction = camera.GetFront();

		// render frame
		windowManager.ClearScreen();

		// render cube
		entityShader.Bind();

		entityShader.SetUniform("uModel", crate.GetModelMatrix());
		entityShader.SetUniform("uView", camera.GetViewMatrix());
		entityShader.SetUniform("uProjection", camera.GetProjectionMatrix());
		entityShader.SetUniform("uNormal", crate.GetNormalMatrix());
		entityShader.SetUniform("uViewPos", camera.GetPosition());
		entityShader.SetUniform("uMaterial.diffuseMap", 0);
		entityShader.SetUniform("uMaterial.specularMap", 1);
		entityShader.SetUniform("uMaterial.emissionMap", 2);
		entityShader.SetUniform("uMaterial.shininess", crate.GetMaterial()->GetShininess());

		// directional lights
		for (size_t i = 0; i < dirLights.size(); ++i) {
			std::string lightUniformName = "uDirLights[" + std::to_string(i) + "]";
			entityShader.SetUniform(lightUniformName + ".direction", dirLights[i].direction);
			entityShader.SetUniform(lightUniformName + ".color.ambient", dirLights[i].color.ambient);
			entityShader.SetUniform(lightUniformName + ".color.diffuse", dirLights[i].color.diffuse);
			entityShader.SetUniform(lightUniformName + ".color.specular", dirLights[i].color.specular);
		}

		// point lights
		for (size_t i = 0; i < pointLights.size(); ++i) {
			std::string lightUniformName = "uPointLights[" + std::to_string(i) + "]";
			entityShader.SetUniform(lightUniformName + ".position", pointLights[i].position);
			entityShader.SetUniform(lightUniformName + ".constant", pointLights[i].constant);
			entityShader.SetUniform(lightUniformName + ".linear", pointLights[i].linear);
			entityShader.SetUniform(lightUniformName + ".quadratic", pointLights[i].quadratic);
			entityShader.SetUniform(lightUniformName + ".color.ambient", pointLights[i].color.ambient);
			entityShader.SetUniform(lightUniformName + ".color.diffuse", pointLights[i].color.diffuse);
			entityShader.SetUniform(lightUniformName + ".color.specular", pointLights[i].color.specular);
		}

		// spot lights
		for (size_t i = 0; i < spotLights.size(); ++i) {
			std::string lightUniformName = "uSpotLights[" + std::to_string(i) + "]";
			entityShader.SetUniform(lightUniformName + ".direction", spotLights[i].direction);
			entityShader.SetUniform(lightUniformName + ".cutOff", spotLights[i].cutOff);
			entityShader.SetUniform(lightUniformName + ".outerCutOff", spotLights[i].outerCutOff);
			entityShader.SetUniform(lightUniformName + ".pl.position", spotLights[i].pl.position);
			entityShader.SetUniform(lightUniformName + ".pl.constant", spotLights[i].pl.constant);
			entityShader.SetUniform(lightUniformName + ".pl.linear", spotLights[i].pl.linear);
			entityShader.SetUniform(lightUniformName + ".pl.quadratic", spotLights[i].pl.quadratic);
			entityShader.SetUniform(lightUniformName + ".pl.color.ambient", spotLights[i].pl.color.ambient);
			entityShader.SetUniform(lightUniformName + ".pl.color.diffuse", spotLights[i].pl.color.diffuse);
			entityShader.SetUniform(lightUniformName + ".pl.color.specular", spotLights[i].pl.color.specular);
		}

		entityShader.SetUniform("uNumLightsMask", numLightsMask);

		crate.GetMaterial()->GetDiffuse()->Bind(0);
		crate.GetMaterial()->GetSpecular()->Bind(1);
		//crate.GetMaterial()->GetEmission()->Bind(2);

		crate.GetMesh()->Bind();
		crate.GetMesh()->Draw();
		crate.GetMesh()->Unbind();

		entityShader.Unbind();

		// render light source entities
		lightSourceShader.Bind();
		lightSourceShader.SetUniform("uView", camera.GetViewMatrix());
		lightSourceShader.SetUniform("uProjection", camera.GetProjectionMatrix());
		for (size_t i = 0; i < lightSourceEntities.size(); ++i) {
			if (i >= pointLights.size()) continue;

			const auto& mesh = lightSourceEntities[i]->GetMesh();
			if (mesh == nullptr) continue;

			lightSourceShader.SetUniform("uModel", lightSourceEntities[i]->GetModelMatrix());
			lightSourceShader.SetUniform("uColor", pointLights[i].color.specular);

			mesh->Bind();
			mesh->Draw();
			mesh->Unbind();
		}
		lightSourceShader.Unbind();

		// update window
		windowManager.Update();
	}

	// terminate
	windowManager.Cleanup();
	return 0;
}