#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene2.h"
#include "MMath.h"
#include "Matrix.h"
#include "Vector.h"
#include "CameraActor.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "BodyComponent.h"
#include "AIComponent.h"
#include "NetworkManager.h"
#include "PhysicsCollision.h"
#include "CollisionComponent.h"
#include "WallActor.h"
#include "Graph.h"
#include "Node.h"
#include "Path.h"
#include "User.h"
#include "LightActor.h"
#include "CameraActor.h"

Scene2::Scene2(): SceneActor(nullptr){
	assetManager = new AssetManager("Scene2");
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);

}

bool Scene2::OnCreate() {
	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);
	if (isCreated) return true;
	SceneActor::OnCreate();

	playerName = "MarioBlack";

	if (!OnCreate_Scene()) return false;

	return true;
}

void Scene2::Update(const float deltaTime){


	Ref<Actor> mario = GetComponent<Actor>("Mario");
	Ref<TransformComponent> marioTransform = mario->GetComponent<TransformComponent>();

	Ref<Actor> marioblack = GetComponent<Actor>("MarioBlack");
	Ref<BodyComponent> body_ = marioblack->GetComponent<BodyComponent>();

	if (NetworkManager::getInstance()->isConnect()) {
		if (NetworkManager::getInstance()->Receive()) {
			ActorData* actorData = NetworkManager::getInstance()->getReceive();
			for (auto actor_ : actorList) {

				//Don't update position for camera and light
				if (strcmp(actor_.first, "camera") == 0 || strcmp(actor_.first, "light") == 0) {
					continue;
				}
				if (!actorData) continue;
				if (strcmp(actorData->actorName, actor_.first) == 0) {
					Ref<BodyComponent> actorbody = actor_.second->GetComponent<BodyComponent>();
					actorbody->setPos(actorData->actorPos);
				}
				//Only server should send data frequently
				//Client should only send data when their character make a movement
				//Update all the actor, such as npc and objects, except for player
				if (NetworkManager::getInstance()->getUserType() == UserType::SERVER) {
					NetworkManager::getInstance()->Send(actor_.first, actor_.second);
				}

			}
		}
	}


	SceneActor::Update(deltaTime);
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent){
	Ref<CameraActor> camera = GetComponent<CameraActor>("camera");
	camera->HandleEvents(sdlEvent);
	Ref<Actor> mario_black = GetComponent<Actor>(playerName);
	Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();

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
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			body_->setPos(body_->getBody()->getPos() + Vec3(0.0f, 0.1f, 0.0));
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send(playerName, mario_black);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			body_->setPos(body_->getBody()->getPos() + Vec3(-0.1f, 0.0f, 0.0));
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send(playerName, mario_black);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			body_->setPos(body_->getBody()->getPos() + Vec3(0.0f, -0.1f, 0.0));
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send(playerName, mario_black);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			body_->setPos(body_->getBody()->getPos() + Vec3(0.1f, 0.0f, 0.0));
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send(playerName, mario_black);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Z) {
			body_->setOrientation(body_->getBody()->getOrientation() + -0.1f);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_X) {
			Ref<Actor> mario_black = GetComponent<Actor>(playerName);
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setOrientation(body_->getBody()->getOrientation() + 0.1f);
		}
		break;

	case SDL_MOUSEMOTION:
		break;
	}
}





	
