#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1.h"
#include "MMath.h"
#include "Debug.h"
#include "CameraActor.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "LightActor.h"

Scene1::Scene1() :camera{ nullptr }, gameboard{ nullptr }, light{ nullptr }, checker_red{ nullptr }, checker_black{ nullptr }{
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {
	Debug::Info("Deleted Scene1: ", __FILE__, __LINE__);

}

bool Scene1::OnCreate() {
	Debug::Info("Loading assets Scene1: ", __FILE__, __LINE__);
	camera = std::make_shared<CameraActor>(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -20.0f), Quaternion());
	camera->OnCreate();
	camera->GetProjectionMatrix().print("ProjectionMatrix");
	camera->GetViewMatrix().print("ViewMatrix");

	light = std::make_shared<LightActor>(nullptr, LightStyle::DirectionLight, Vec3(0.0f, 5.0f, -10.0f), Vec4(0.6f, 0.6f, 0.6f, 0.0f));
	light->OnCreate();

	//Quaternion(0.87f, -0.5f, 0.0f, 0.0f)
	gameboard = std::make_shared<Actor>(nullptr);
	gameboard->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion());
	gameboard->AddComponent<MeshComponent>(nullptr,"meshes/Plane.obj");
	gameboard->AddComponent<ShaderComponent>(nullptr,"shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	gameboard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	gameboard->OnCreate();

	checker_red = std::make_shared<Actor>(gameboard);
	checker_red->AddComponent<TransformComponent>(nullptr, Vec3(-0.65f, -4.3f, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
	checker_red->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
	checker_red->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	checker_red->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png");
	checker_red->OnCreate();

	checker_black = std::make_shared<Actor>(gameboard);
	checker_black->AddComponent<TransformComponent>(nullptr, Vec3(0.63f, 4.4f, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
	checker_black->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj");
	checker_black->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	checker_black->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png");
	checker_black->OnCreate();

	
	return true;
}

void Scene1::OnDestroy() {
	Debug::Info("Deleting assets Scene1: ", __FILE__, __LINE__);
	//if(camera) delete camera;
	//if(light) delete light;
	//if(gameboard) delete gameboard;
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
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
			Ref<TransformComponent> transform_ = gameboard->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			std::cout << "D EVENT\n";
			Ref<TransformComponent> transform_ = gameboard->GetComponent<TransformComponent>();
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

void Scene1::Update(const float deltaTime) {

}

void Scene1::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Ref<MaterialComponent> texture = gameboard->GetComponent<MaterialComponent>();
	Ref<ShaderComponent> shader = gameboard->GetComponent<ShaderComponent>();
	Ref<MeshComponent> mesh = gameboard->GetComponent<MeshComponent>();
	if (shader == nullptr || mesh == nullptr) {
		return;
	}

	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, gameboard->GetModelMatrix());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightDataID());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);
	
	//Render Checkpiece red
	texture = checker_red->GetComponent<MaterialComponent>();
	shader = checker_red->GetComponent<ShaderComponent>();
	mesh = checker_red->GetComponent<MeshComponent>();
	if (shader == nullptr || mesh == nullptr) {
		return;
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checker_red->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);

	//Render Checkpiece black
	texture = checker_black->GetComponent<MaterialComponent>();
	shader = checker_black->GetComponent<ShaderComponent>();
	mesh = checker_black->GetComponent<MeshComponent>();
	if (shader == nullptr || mesh == nullptr) {
		return;
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checker_black->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);


	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}



	
