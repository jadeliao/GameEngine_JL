#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "Debug.h"
#include "CameraActor.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "LightActor.h"

Scene0::Scene0() :camera{ nullptr }, mario{ nullptr }, light{ nullptr }{
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);

}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	camera = std::make_shared<CameraActor>(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -20.0f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	light = std::make_shared<LightActor>(nullptr, LightStyle::DirectionLight, Vec3(-10.0f, 0.0f, 0.0f), Vec4(0.8f, 0.4f, 0.4f, 0.0f));
	light->OnCreate();

	mario = std::make_shared<Actor>(nullptr);
	mario->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 7.0f), QMath::angleAxisRotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)));
	mario->AddComponent<MeshComponent>(nullptr,"meshes/Mario.obj");
	mario->AddComponent<ShaderComponent>(nullptr,"shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	mario->AddComponent<MaterialComponent>(nullptr, "textures/mario_main.png");
	mario->OnCreate();

	hammer = std::make_shared<Actor>(mario);
	hammer->AddComponent<TransformComponent>(nullptr, Vec3(-1.0f, 0.0f, 0.0f), QMath::angleAxisRotation(-90.0f, Vec3(1.0f, 0.0f, 0.0f)));
	hammer->AddComponent<MeshComponent>(nullptr, "meshes/Hammer.obj");
	hammer->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	hammer->AddComponent<MaterialComponent>(nullptr, "textures/hammer_BaseColor.png");
	hammer->OnCreate();
	
	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	//if(camera) delete camera;
	//if(light) delete light;
	//if(mario) delete mario;
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			std::cout << "LEFT EVENT\n";
			Ref<TransformComponent> transform_ = camera->GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(-1.0f, 0.0f, 0.0f), transform_->GetQuaternion());
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			std::cout << "RIGHT EVENT\n";
			Ref<TransformComponent> transform_ = camera->GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(1.0f, 0.0f, 0.0f), transform_->GetQuaternion());
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			std::cout << "UP EVENT\n";
			Ref<TransformComponent> transform_ = camera->GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, 1.0f), transform_->GetQuaternion());
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			std::cout << "DOWN EVENT\n";
			Ref<TransformComponent> transform_ = camera->GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, -1.0f), transform_->GetQuaternion());
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
			std::cout << "Q EVENT\n";
			Ref<TransformComponent> transform_ = camera->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E) {
			std::cout << "E EVENT\n";
			Ref<TransformComponent> transform_ = camera->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			std::cout << "A EVENT\n";
			Ref<TransformComponent> transform_ = mario->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			std::cout << "D EVENT\n";
			Ref<TransformComponent> transform_ = mario->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		break;

	case SDL_MOUSEMOTION:          
		break;

	case SDL_MOUSEBUTTONDOWN:              
		break; 

	case SDL_MOUSEBUTTONUP:            
	break;

	default:
		break;
    }
}

void Scene0::Update(const float deltaTime) {
	//This is for rotating Mario constantly
	//static float rotation = 0.0f;
	//rotation += 1.0f;
	//TransformComponent *transformation = mario->GetComponent<TransformComponent>();
	//Quaternion orientation = transformation->GetQuaternion();
	//Vec3 pos = transformation->GetPosition();
	//transformation->SetTransform(pos, QMath::angleAxisRotation(rotation, Vec3(0.0f, 1.0f, 0.0f)));
}

void Scene0::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Ref<MaterialComponent> texture = mario->GetComponent<MaterialComponent>();
	Ref<ShaderComponent> shader = mario->GetComponent<ShaderComponent>();
	Ref<MeshComponent> mesh = mario->GetComponent<MeshComponent>();
	Ref<TransformComponent> transform = mario->GetComponent<TransformComponent>();
	if (shader == nullptr || mesh == nullptr || transform == nullptr) {
		return;
	}

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, mario->GetModelMatrix());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightDataID());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);

	texture = hammer->GetComponent<MaterialComponent>();
	shader = hammer->GetComponent<ShaderComponent>();
	mesh = hammer->GetComponent<MeshComponent>();
	transform = hammer->GetComponent<TransformComponent>();
	if (shader == nullptr || mesh == nullptr || transform == nullptr) {
		return;
	}

	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, hammer->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
