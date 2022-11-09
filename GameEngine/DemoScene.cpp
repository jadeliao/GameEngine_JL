#include <glew.h>
#include <SDL.h>
#include "Debug.h"
#include "DemoScene.h"
#include "MMath.h"
#include "Matrix.h"
#include "Vector.h"
#include "CameraActor.h"
#include "AssetManager.h"
#include "TransformComponent.h"
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
	SceneActor::OnCreate();
	
	//Set the target for AI
	Ref<Actor> mario = GetComponent<Actor>("Mario");
	Ref<Actor> marioblack = GetComponent<Actor>("MarioBlack");
	Ref<AIComponent> mario_ai = mario->GetComponent<AIComponent>();
	mario_ai->setTarget(marioblack);

	if (!OnCreate_Scene()) return false;

	return true;
}

void DemoScene::Update(const float deltaTime) {
	SceneActor::Update(deltaTime);
}

void DemoScene::HandleEvents(const SDL_Event& sdlEvent){

	Ref<CameraActor> camera = GetComponent<CameraActor>("camera");
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
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setPos(body_->getBody()->getPos() + Vec3(0.0f, 0.1f, 0.0));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setPos(body_->getBody()->getPos() + Vec3(-0.1f, 0.0f, 0.0));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setPos(body_->getBody()->getPos() + Vec3(0.0f, -0.1f, 0.0));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setPos(body_->getBody()->getPos() + Vec3(0.1f, 0.0f, 0.0));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Z) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setOrientation(body_->getBody()->getOrientation() + -0.1f);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_X) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setOrientation(body_->getBody()->getOrientation() + 0.1f);
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
