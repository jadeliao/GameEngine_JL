#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "DemoScene.h"
#include "MMath.h"
#include "Debug.h"
#include "CameraActor.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "LightActor.h"
#include "BodyComponent.h"
#include "AIComponent.h"

DemoScene::DemoScene() : SceneActor(nullptr) {
	assetManager = new AssetManager("DemoScene");
	Debug::Info("Created DemoScene: ", __FILE__, __LINE__);
}

DemoScene::~DemoScene() {
	Debug::Info("Deleted DemoScene: ", __FILE__, __LINE__);

}

bool DemoScene::OnCreate() {
	Debug::Info("Loading assets DemoScene: ", __FILE__, __LINE__);
	if (isCreated) return true;

	Ref<Actor> mario = assetManager->GetComponent<Actor>("Mario");
	Ref<Actor> marioblack = assetManager->GetComponent<Actor>("MarioBlack");

	Vec3 mario_pos(-10.0f, -7.0f, 0.0f);
	Vec3 mario_black_pos(0.0f, 0.0f, 0.0f);

	Ref<TransformComponent> mario_transform = mario->GetComponent<TransformComponent>();
	mario_transform->SetTransform(mario_pos, 
		Quaternion() * QMath::angleAxisRotation(mario->GetComponent<BodyComponent>()->getBody()->getOrientation(), 
		Vec3(0.0f, 0.0f, 1.0f)), Vec3(1.0f, 1.0f, 1.0f));
	Ref<TransformComponent> mario_black_transform = marioblack->GetComponent<TransformComponent>();

	//Set orientation and position
	Ref<BodyComponent> black_body = marioblack->GetComponent<BodyComponent>();
		mario_black_transform->SetTransform(mario_black_pos, 
		Quaternion() * QMath::angleAxisRotation(black_body->getBody()->getOrientation(),
		Vec3(0.0f, 0.0f, 1.0f)), Vec3(1.0f, 1.0f, 1.0f));
	//checker_red->GetComponent<BodyComponent>()->setPos(red_pos);
	//checker_white->GetComponent<BodyComponent>()->setPos(white_pos);
	mario->GetComponent<BodyComponent>()->setPos(mario_pos);
	//mario->GetComponent<BodyComponent>()->setOrientation(QMath::magnitude(mario_transform->GetQuaternion()));
	marioblack->GetComponent<BodyComponent>()->setPos(mario_black_pos);
	//marioblack->GetComponent<BodyComponent>()->setOrientation(QMath::magnitude(mario_black_transform->GetQuaternion()));
	//Add AI Component
	//Ref<AIComponent> red_ai = std::make_shared<AIComponent>(checker_red, seek, checker_white);
	//checker_red->AddComponent<AIComponent>(red_ai);
	
	Ref<AIComponent> mario_ai = std::make_shared<AIComponent>(mario, marioblack);
	mario->AddComponent<AIComponent>(mario_ai);
	AddActor("Mario", mario);
	AddActor("MarioBlack", marioblack);


	if (!OnCreate_Scene()) return false;

	return true;
}

void DemoScene::Update(const float deltaTime) {
	SceneActor::Update(deltaTime);
	//Ref<Actor> mario = assetManager->GetComponent<Actor>("Mario");
	//Ref<Actor> marioblack = assetManager->GetComponent<Actor>("MarioBlack");
	//marioblack->GetComponent<TransformComponent>()->GetQuaternion().print();
	//mario->GetComponent<TransformComponent>()->GetQuaternion().print();
	//cout << "orientation: " << mario->GetComponent<BodyComponent>()->getBody()->getOrientation() << endl;
}

void DemoScene::HandleEvents(const SDL_Event& sdlEvent){

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
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Z) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setOrientation(body_->getBody()->getOrientation() + -2.0f);

			//std::cout << body_->getBody()->getOrientation() << std::endl;

			Ref<TransformComponent> transform_ = mario_black->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 0.0f, 1.0f)));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_X) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setOrientation(body_->getBody()->getOrientation() + 2.0f);

			//std::cout << body_->getBody()->getOrientation() << std::endl;

			Ref<TransformComponent> transform_ = mario_black->GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 0.0f, 1.0f)));
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
