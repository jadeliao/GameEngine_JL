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
#include "PhysicsCollision.h"
#include "WallActor.h"
#include "Graph.h"
#include "Node.h"


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

	//Ref<WallActor> wall = std::make_shared<WallActor>(nullptr);
	//Vec3 size(1.0f, 1.0f, 1.0f);
	//Vec3 pos(1.0f, 0.0f, 0.0f);
	//Ref<Box> shape_ = std::make_shared<GEOMETRY::Box>(pos, size, Quaternion());
	//Ref<TransformComponent> transform_ = std::make_shared<TransformComponent>(wall);
	//Ref<CollisionComponent> collision_ = std::make_shared<CollisionComponent>(wall, "box", shape_);
	//wall->AddComponent<CollisionComponent>(collision_);
	//wall->AddComponent<TransformComponent>(transform_);
	//AddActor("Wall", wall);
	//wallTransform->SetTransform(wallTile->centre, Quaternion());
	
	//transform_->SetTransform(Vec3(5.0f, 2.0f, 1.0f), Quaternion());

	//Set the target for AI
	//Ref<Actor> mario = GetComponent<Actor>("Mario");
	//Ref<Actor> marioblack = GetComponent<Actor>("MarioBlack");
	//Ref<AIComponent> mario_ai = mario->GetComponent<AIComponent>();
	//mario_ai->setTarget(marioblack);



	if (!OnCreate_Scene()) return false;

	return true;
}

void DemoScene::Update(const float deltaTime) {
	SceneActor::Update(deltaTime);
	actInterval -= deltaTime;
	Ref<Actor> mario = GetComponent<Actor>("Mario");
	Ref<BodyComponent> mariobody = mario->GetComponent<BodyComponent>();

	//Make actor move
	if (actInterval < 0.0f) {
		if (!pathList.empty()) {
			//Get the first item in the list
			int i = pathList.front();
			Ref<Node> node_ = graph->getNode(i);
			Vec3 newPos = node_->getPos();
			mariobody->setPos(newPos);
			actInterval = 0.5f;
			pathList.erase(pathList.begin());
		}
	}
}

void DemoScene::HandleEvents(const SDL_Event& sdlEvent){

	Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
	Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();

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
			body_->setPos(body_->getBody()->getPos() + Vec3(0.0f, 0.1f, 0.0));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			body_->setPos(body_->getBody()->getPos() + Vec3(-0.1f, 0.0f, 0.0));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			body_->setPos(body_->getBody()->getPos() + Vec3(0.0f, -0.1f, 0.0));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			body_->setPos(body_->getBody()->getPos() + Vec3(0.1f, 0.0f, 0.0));

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Z) {
			body_->setOrientation(body_->getBody()->getOrientation() + -0.1f);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_X) {
			Ref<Actor> mario_black = GetComponent<Actor>("MarioBlack");
			Ref<BodyComponent> body_ = mario_black->GetComponent<BodyComponent>();
			body_->setOrientation(body_->getBody()->getOrientation() + 0.1f);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_G) {
			pathList = graph->AStar(0, 133);
			for (int i : pathList) {
				std::cout << i << " ";
			}
			std::cout << endl;


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
