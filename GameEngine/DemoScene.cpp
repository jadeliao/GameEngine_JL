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

	//Add actors
	Ref<Actor> checker_red = assetManager->GetComponent<Actor>("RedChecker");
	Ref<Actor> checker_white = assetManager->GetComponent<Actor>("WhiteChecker");
	Vec3 red_pos(-12.0f, -7.0f, 0.0f);
	Vec3 white_pos(0.0f, 0.0f, 0.0f);
	Ref<TransformComponent> checker_red_transform = checker_red->GetComponent<TransformComponent>();
	checker_red_transform->SetTransform(red_pos, Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
	Ref<TransformComponent> checker_white_transform = checker_white->GetComponent<TransformComponent>();
	checker_white_transform->SetTransform(white_pos, Quaternion(), Vec3(0.15f, 0.15f, 0.15f));
	checker_red->GetComponent<BodyComponent>()->setPos(red_pos);
	checker_white->GetComponent<BodyComponent>()->setPos(white_pos);
	Ref<AIComponent> red_ai = std::make_shared<AIComponent>(checker_red, seek, checker_white);
	checker_red->AddComponent<AIComponent>(red_ai);
	AddActor("redchecker", checker_red);
	AddActor("whitechecker", checker_white);


	if (!OnCreate_Scene()) return false;

	return true;
}

void DemoScene::Update(const float deltaTime) {
	SceneActor::Update(deltaTime);
	//Ref<Actor> red_checker = assetManager->GetComponent<Actor>("RedChecker");
	//Ref<Body> red_checker_body = red_checker->GetComponent<BodyComponent>()->getBody();
	//Ref<Body> target = assetManager->GetComponent<Actor>("WhiteChecker")->GetComponent<BodyComponent>()->getBody();
	//Ref<KinematicSeek> steering_algorithm;
	//steering_algorithm = std::make_shared<KinematicSeek>(red_checker_body, target);
	//Ref<KinematicSteeringOutput> steering;
	//steering = steering_algorithm->getSteering();
	//red_checker_body->Update();;

}

void DemoScene::HandleEvents(const SDL_Event& sdlEvent){
}
