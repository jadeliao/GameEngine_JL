#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene2.h"
#include "MMath.h"
#include "Debug.h"
#include "CameraActor.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "LightActor.h"

Scene2::Scene2(): SceneActor(nullptr){
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);

}

bool Scene2::OnCreate() {
	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);
	if (isCreated) return true;

	Ref<CameraActor> camera = std::make_shared<CameraActor>(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -20.0f), Quaternion());
	camera->OnCreate();
	AddActor("camera", camera);

	Ref<LightActor> light = std::make_shared<LightActor>(nullptr, LightStyle::DirectionLight, Vec3(0.0f, 5.0f, -10.0f), Vec4(0.6f, 0.6f, 0.6f, 0.0f));
	light->OnCreate();
	AddActor("light", light);

	//Quaternion(0.87f, -0.5f, 0.0f, 0.0f)
	Ref<Actor> gameboard = assetManager->GetComponent<Actor>("Gameboard");
	Ref<Actor> checker_red_temp = assetManager->GetComponent<Actor>("RedChecker");
	Ref<Actor> checker_black_temp = assetManager->GetComponent<Actor>("BlackChecker");

	gameboard->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion());
	AddActor("Gameboard", gameboard);

	//Declare the positions
	int row_count = 0;
	int column_count = 0;
	float grid_interval = 1.25f;
	float edge = 4.4f;
	std::string redCheckName;
	std::string blackCheckName;
	//Create the checkers and add it to the list
	for (int i = 0; i < CHECKER_MAX_NUM; i++) {

		//Determine the location where red piece is placed
		float red_x = (row_count * grid_interval * 2.0f) - ((column_count % 2) * -grid_interval) - edge;
		float red_y = (column_count * grid_interval) - edge;
		//Create a copy of the red checker and add it to the actor list
		Ref<Actor> checker_red = std::make_shared<Actor>(*(checker_red_temp.get()));
		checker_red->AddComponent<TransformComponent>(nullptr, Vec3(red_x, red_y, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
		checker_red->OnCreate();
		redCheckName = "redCheck" + (std::to_string(i));
		AddActor(redCheckName.c_str(), checker_red);

		//Determine the location where black piece is placed
		float black_x = (row_count * grid_interval * 2.0f) - ((column_count % 2) * -grid_interval) - edge;
		float black_y = (column_count * -grid_interval) + edge;
		//Create a copy of the black checker and add it to the actor list
		Ref<Actor> checker_black = std::make_shared<Actor>(*(checker_black_temp.get()));
		checker_black->AddComponent<TransformComponent>(nullptr, Vec3(black_x, black_y, 0.0f), Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
		checker_black->OnCreate();
		blackCheckName = "blackCheck" + (std::to_string(i));
		AddActor(blackCheckName.c_str(), checker_black);

		//Update row count and column count
		row_count++;
		if (row_count >= 4) {
			column_count++;
			row_count = 0;
		}
	}

	if (!OnCreate_Scene()) return false;

	return true;
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent){
	Ref<CameraActor> camera = GetComponent<CameraActor>("camera");
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			std::cout << "LEFT EVENT\n";
			//Ref<CameraActor> camera = GetComponent<CameraActor>("camera");
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
			Ref<Actor> actorControl = GetComponent<Actor>("Gameboard");
			Ref<TransformComponent> transform_ = actorControl->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			std::cout << "D EVENT\n";
			Ref<Actor> actorControl = GetComponent<Actor>("Gameboard");
			Ref<TransformComponent> transform_ = actorControl->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			std::cout << "W EVENT\n";
			Ref<Actor> actorControl = GetComponent<Actor>("Gameboard");
			Ref<TransformComponent> transform_ = actorControl->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(1.0f, 0.0f, 0.0f)));

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			std::cout << "S EVENT\n";
			Ref<Actor> actorControl = GetComponent<Actor>("Gameboard");
			Ref<TransformComponent> transform_ = actorControl->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(1.0f, 0.0f, 0.0f)));
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





	
