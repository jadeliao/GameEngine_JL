#include <glew.h>
#include <SDL.h>
#include <thread>
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

#define START 0
#define GOAL 133 //133

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
	//Ref<AIComponent> mario_ai = mario->GetComponent<AIComponent>();
	//mario_ai->setTarget(marioblack);

	if (!OnCreate_Scene()) return false;
	//Set black mario to goal location
	Ref<Actor> marioblack = GetComponent<Actor>("MarioBlack");
	Vec3 newPos = graph->getNode(GOAL)->getPos();
	Ref<BodyComponent> blackBody_ = marioblack->GetComponent<BodyComponent>();
	blackBody_->setPos(newPos);

	//Set mario to start location
	Ref<Actor> mario = GetComponent<Actor>("Mario");
	newPos = graph->getNode(START)->getPos();
	Ref<BodyComponent> body_ = mario->GetComponent<BodyComponent>();
	body_->setPos(newPos);

	return true;
}

void DemoScene::Update(const float deltaTime) {
	SceneActor::Update(deltaTime);
	actInterval -= deltaTime;
	Ref<Actor> mario = GetComponent<Actor>("Mario");
	Ref<TransformComponent> marioTransform = mario->GetComponent<TransformComponent>();

	Ref<Actor> marioblack = GetComponent<Actor>("MarioBlack");
	Ref<BodyComponent> body_ = marioblack->GetComponent<BodyComponent>();

	for (auto actor_ : actorList) {
		if (NetworkManager::getInstance()->isConnect()) {
			//Don't update position for camera and light
			if (strcmp(actor_.first, "camera") == 0 || strcmp(actor_.first, "light") == 0) {
				continue;
			}
			if (NetworkManager::getInstance()->Receive()) {
				ActorData* actorData = NetworkManager::getInstance()->getReceive();
				if (!actorData) continue;
				if (strcmp(actorData->actorName, actor_.first) == 0) {
					Ref<BodyComponent> actorbody = actor_.second->GetComponent<BodyComponent>();
					actorbody->setPos(actorData->actorPos);
				}
			}
			//Only server should send data frequently
			//Client should only send data when their character make a movement
			//NPC control is server's responsibility
			if (NetworkManager::getInstance()->getUserType() == UserType::SERVER) {
				NetworkManager::getInstance()->Send(actor_.first, actor_.second);
			}
		}
	}

	//Ref<Actor> actor_ = std::dynamic_pointer_cast<Actor>(component_);
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
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send("MarioBlack", mario_black);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			body_->setPos(body_->getBody()->getPos() + Vec3(-0.1f, 0.0f, 0.0));
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send("MarioBlack", mario_black);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			body_->setPos(body_->getBody()->getPos() + Vec3(0.0f, -0.1f, 0.0));
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send("MarioBlack", mario_black);
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			body_->setPos(body_->getBody()->getPos() + Vec3(0.1f, 0.0f, 0.0));
			Ref<TransformComponent> actorTransform = mario_black->GetComponent<TransformComponent>();
			Vec3 actorPos = actorTransform->GetPosition();
			NetworkManager::getInstance()->Send("MarioBlack", mario_black);
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
			//Set the position
			Ref<Actor> mario = GetComponent<Actor>("Mario");
			Ref<AIComponent> mario_AI = mario->GetComponent<AIComponent>();
			Ref<BodyComponent> marioBody_ = mario->GetComponent<BodyComponent>();
			Vec3 newPos = graph->getNode(START)->getPos();
			marioBody_->setPos(newPos);
			//Start path finding with AStar
			graphConnection();
			std::vector<Ref<Node>> pathList_ = graph->AStar(START, GOAL);

			if (pathList_.empty()) {
				std::cout << "No Path is found";
			}
			else {
				//Set Path for path following

				Ref<Path> path_ = std::make_shared<Path>(pathList_);
				mario_AI->setPath(path_);
			}
		}

		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			//Get mouse coordinates
			Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
			//Get the matrix used to transform mouse coordinates to world space
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			Matrix4 ndc = MMath::viewportNDC(viewport[2], viewport[3]);
			Matrix4 projection = camera->GetProjectionMatrix();
			Matrix4 view = camera->GetViewMatrix();
			Matrix4 rayTransform = MMath::inverse(ndc * projection * view);

			//Tranform mouse coordinates into world coorinates using above matrix
			//Then normalize the vector to get the ray direction in world space

			Vec3 mouseWorldCoords = rayTransform * Vec3(mouseCoords.x, mouseCoords.y, 1.0f);
			//Set ray
			Vec3 rayWorldStart = camera->GetComponent<TransformComponent>()->GetPosition();
			Vec3 rayWorldDir = VMath::normalize(mouseWorldCoords);
			Ray ray(rayWorldStart, rayWorldDir);

			// Loop through all the actors and check if the ray has collided with them
			// Pick the one with the smallest positive t value
			int rows = wallList.size();
			int columns = wallList[0].size();
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {

					Ref<WallActor> actor = wallList[i][j]->getTile();
					Ref<TransformComponent> transformComponent = actor->GetComponent<TransformComponent>();
					Ref<CollisionComponent> collisionComponent = actor->GetComponent<CollisionComponent>();
					if (collisionComponent->shapeType == ShapeType::sphere
						|| collisionComponent->shapeType == ShapeType::cylinder
						|| collisionComponent->shapeType == ShapeType::capsule
						|| collisionComponent->shapeType == ShapeType::box
						) {
						// Transform the ray into the local space of the object and check if a collision occured
						Vec3 rayLocalStart = MMath::inverse(actor->GetModelMatrix()) * ray.start;
						Vec3 rayLocalDir = MMath::inverse(actor->GetModelMatrix()).multiplyWithoutDividingOutW(Vec4(ray.dir, 0.0f));
						Ray rayLocal(rayLocalStart, rayLocalDir);
						RayIntersectionInfo rayInfo = collisionComponent->shape->rayIntersectionInfo(rayLocal);

						if (rayInfo.isIntersected) {
							//Transform world intersection point into object related coordinates
							actor->setVisible(!actor->getVisible());
							std::cout << "Intersect: " << wallList[i][j]->getLabel() << endl;		
						}
						else {
							//std::cout << "You picked nothing\n";
						}
					}
				}
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}
}
