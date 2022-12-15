#include <glew.h>
#include <SDL.h>
#include "Debug.h"
#include "DemoScene2.h"
#include "MMath.h"
#include "Matrix.h"
#include "Vector.h"
#include "CameraActor.h"
#include "AssetManager.h"
#include "TransformComponent.h"
#include "BodyComponent.h"
#include "AIComponent.h"
#include "PhysicsCollision.h"
#include "CollisionComponent.h"
#include "WallActor.h"
#include "Graph.h"
#include "Node.h"
#include "Path.h"
#include "StateMachine.h"
#include "State.h"
#include "Transition.h"
#include "DirectionCondition.h"
#include "IdleCondition.h"
#include "AnimationAction.h"
#include "AnimationComponent.h"

#define START 0
#define GOAL 119 //133

DemoScene2::DemoScene2() : SceneActor(nullptr) {
	assetManager = new AssetManager("DemoScene2");
	Debug::Info("Created DemoScene2: ", __FILE__, __LINE__);
}

DemoScene2::~DemoScene2() {
	Debug::Info("Deleted DemoScene2: ", __FILE__, __LINE__);

}

bool DemoScene2::OnCreate() {
	Debug::Info("Loading assets DemoScene2: ", __FILE__, __LINE__);
	if (isCreated) return true;
	SceneActor::OnCreate();

	if (!OnCreate_Scene()) return false;
	//Set black mario to goal location
	Ref<Actor> marioblack = GetComponent<Actor>("MarioBlack");
	Vec3 newPos = graph->getNode(GOAL)->getPos();
	Ref<TransformComponent> blackTransform_ = marioblack->GetComponent<TransformComponent>();
	blackTransform_->SetTransform(blackTransform_->GetPosition(), blackTransform_->GetQuaternion(), Vec3(2.0f, 2.0f, 2.0f));
	Ref<BodyComponent> blackBody_ = marioblack->GetComponent<BodyComponent>();
	blackBody_->setPos(newPos);

	//Set mario to start location
	Ref<Actor> mario = GetComponent<Actor>("Mario");
	newPos = graph->getNode(START)->getPos();
	Ref<TransformComponent> transform_ = mario->GetComponent<TransformComponent>();
	transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion(), Vec3(2.0f, 2.0f, 2.0f));
	Ref<BodyComponent> body_ = mario->GetComponent<BodyComponent>();
	body_->setPos(newPos);

	//Set StateMachine for the actor
	//Ref<AnimationComponent> actorAnimation = mario->GetComponent<AnimationComponent>();
	//actorAnimation->setAnimation("Forward");
	//Ref<StateMachine> stateMachine_ = std::make_shared<StateMachine>();
	//stateMachine_->setOwner(mario);
	////Create action
	//Ref<AnimationAction> IdleAnimation = std::make_shared<AnimationAction>(mario);
	//Ref<AnimationAction> ForwardAnimation = std::make_shared<AnimationAction>(mario);
	//Ref<AnimationAction> BackwardAnimation = std::make_shared<AnimationAction>(mario);
	//Ref<AnimationAction> RightAnimation = std::make_shared<AnimationAction>(mario);
	//Ref<AnimationAction> LeftAnimation = std::make_shared<AnimationAction>(mario);

	//IdleAnimation->setAnimationName("Idle");
	//ForwardAnimation->setAnimationName("Forward");
	//BackwardAnimation->setAnimationName("Backward");
	//RightAnimation->setAnimationName("Right");
	//LeftAnimation->setAnimationName("Left");
	////Create States
	//Ref<State> Idle = std::make_shared<State>();
	//Ref<State> Forward = std::make_shared<State>();
	//Ref<State> Backward = std::make_shared<State>();
	//Ref<State> Right = std::make_shared<State>();
	//Ref<State> Left = std::make_shared<State>();

	//Idle->setAction(IdleAnimation);
	//Forward->setAction(ForwardAnimation);
	//Backward->setAction(BackwardAnimation);
	//Right->setAction(RightAnimation);
	//Left->setAction(LeftAnimation);
	////Create condition
	//Ref<IdleCondition> toIdle = std::make_shared<IdleCondition>(mario);
	//Ref<DirectionCondition> toForward = std::make_shared<DirectionCondition>(mario);
	//toForward->setRange(315.0f, 405.0f);
	//Ref<DirectionCondition> toBackward = std::make_shared<DirectionCondition>(mario);
	//toBackward->setRange(135.0f, 225.0f);
	//Ref<DirectionCondition> toRight = std::make_shared<DirectionCondition>(mario);
	//toRight->setRange(45.0f, 135.0f);
	//Ref<DirectionCondition> toLeft = std::make_shared<DirectionCondition>(mario);
	//toLeft->setRange(225.0f, 315.0f);
	////Create Transition
	//Ref<Transition> transitIdle = std::make_shared<Transition>(Idle, toIdle);
	//Ref<Transition> transitForward = std::make_shared<Transition>(Forward, toForward);
	//Ref<Transition> transitBackward = std::make_shared<Transition>(Backward, toBackward);
	//Ref<Transition> transitRight = std::make_shared<Transition>(Right, toRight);
	//Ref<Transition> transitLeft = std::make_shared<Transition>(Left, toLeft);

	////Add Transition to state
	//Idle->addTransition(transitForward);
	//Idle->addTransition(transitBackward);
	//Idle->addTransition(transitRight);
	//Idle->addTransition(transitLeft);

	//Forward->addTransition(transitIdle);
	//Forward->addTransition(transitBackward);
	//Forward->addTransition(transitRight);
	//Forward->addTransition(transitLeft);

	//Backward->addTransition(transitIdle);
	//Backward->addTransition(transitForward);
	//Backward->addTransition(transitRight);
	//Backward->addTransition(transitLeft);

	//Right->addTransition(transitIdle);
	//Right->addTransition(transitForward);
	//Right->addTransition(transitBackward);
	//Right->addTransition(transitLeft);

	//Left->addTransition(transitIdle);
	//Left->addTransition(transitForward);
	//Left->addTransition(transitBackward);
	//Left->addTransition(transitRight);

	////Set initial state for state machine
	//stateMachine_->setInitialState(Idle);
	//mario->setStateMachine(stateMachine_);

	return true;
}

void DemoScene2::Update(const float deltaTime) {
	SceneActor::Update(deltaTime);
	actInterval -= deltaTime;
	Ref<Actor> mario = GetComponent<Actor>("Mario");
	Ref<BodyComponent> mariobody = mario->GetComponent<BodyComponent>();

}

void DemoScene2::HandleEvents(const SDL_Event& sdlEvent){

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
			Vec3 cameraPos = rayTransform * camera->GetComponent<TransformComponent>()->GetPosition();
			Vec3 rayWorldStart(0.0f, 0.0f, -1.0f);
			Vec3 mouseWorldCoords = rayTransform * (Vec3(mouseCoords.x, mouseCoords.y, 1.0f));

			//Set ray
			Vec3 rayWorldDir = VMath::normalize(mouseWorldCoords);
			Ray ray(rayTransform * rayWorldStart, rayWorldDir);

			// Loop through all the actors and check if the ray has collided with them
			// Pick the one with the smallest positive t value
			int rows = wallList.size();
			int columns = wallList[0].size();
			bool intersected = false;
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
							rayInfo.intersectionPoint.print();
							actor->setVisible(!actor->getVisible());
							std::cout << "Intersect: " << wallList[i][j]->getLabel() << endl;
							intersected = true;
							break;
						}
						else {
							//std::cout << "You picked nothing\n";
						}
					}
				}
				if (intersected) break;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}
}
