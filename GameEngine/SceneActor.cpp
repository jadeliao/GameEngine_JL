#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "SceneActor.h"
#include "MMath.h"
#include "Debug.h"
#include "AssetManager.h"
#include "CameraActor.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "LightActor.h"
#include "NetworkManager.h"
#include "PhysicsCollision.h"
#include "WallActor.h"
#include "Node.h"
#include "Graph.h"

using namespace GEOMETRY;


SceneActor::SceneActor(Ref<Component> parent_) : Actor(parent_), assetManager(nullptr) {

}

bool SceneActor::OnCreate() {
	if (isCreated) return isCreated;
	actorList = assetManager->getActors();
	wallList = assetManager->getWalls();
	isCreated = true;
	return isCreated;
}

bool SceneActor::OnCreate_Scene() {

	Debug::Info("Loading camera for SceneActor: ", __FILE__, __LINE__);
	Ref<CameraActor> camera = std::make_shared<CameraActor>(nullptr);
	camera->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, -20.0f), Quaternion());
	camera->OnCreate();
	AddActor("camera", camera);

	Debug::Info("Loading camera for light: ", __FILE__, __LINE__);
	Ref<LightActor> light = std::make_shared<LightActor>(nullptr, LightStyle::DirectionLight, Vec3(0.0f, 5.0f, -10.0f), Vec4(0.6f, 0.6f, 0.6f, 0.0f));
	light->OnCreate();
	AddActor("light", light);

	Debug::Info("Loading assets for SceneActor: ", __FILE__, __LINE__);
	for (std::pair<const char*, Ref<Actor>> actor : actorList) {
		if (actor.second->OnCreate() == false) {
			Debug::Error("Fail loading assets for SceneActor: ", __FILE__, __LINE__);
			isCreated = false;
		}
	}

	std::vector<Ref<Node>> nodes;
	Debug::Info("Loading walls for SceneActor: ", __FILE__, __LINE__);
	for (std::vector<Ref<Node>> wallRow : wallList) {
		for (Ref<Node> wall : wallRow) {
			if (wall->getTile()->OnCreate() == false) {
				Debug::Error("Fail loading walls for SceneActor: ", __FILE__, __LINE__);
				isCreated = false;
			}
			nodes.push_back(wall);
		}
	}

	//Create graph
	Debug::Info("Loading graph for SceneActor: ", __FILE__, __LINE__);
	graph = std::make_shared<Graph>();
	if (!graph->OnCreate(nodes)) {
		Debug::Error("Fail loading graph for SceneActor: ", __FILE__, __LINE__);
		isCreated = false;
		return isCreated;
	}
	if (!wallList.empty()) graphConnection();


	return isCreated;
}


void SceneActor::graphConnection(){
	int rows = wallList.size();
	int columns = wallList[0].size();
	float width = 2.0f;
	float height = 2.0f;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			int from = wallList[i][j]->getLabel();
			//Only set connections when current node is a valid path
			if (!wallList[i][j]->getTile()->getVisible()) {

				if (j > 0) {
					float weight = 0.0f;
					Ref<Node> toNode = wallList[i][j - 1];
					//Only set weight when the node is valid
					if (!toNode->getTile()->getVisible()) {
						weight = width;
					}
					graph->addWeightConnection(from, toNode->getLabel(), weight);
				}
				if (j < columns - 1) {
					float weight = 0.0f;
					Ref<Node> toNode = wallList[i][j + 1];
					//Only set weight when the node is valid
					if (!toNode->getTile()->getVisible()) {
						weight = width;
					}
					graph->addWeightConnection(from, toNode->getLabel(), weight);
				}

				if (i < rows - 1) {
					float weight = 0.0f;
					Ref<Node> toNode = wallList[i + 1][j];
					//Only set weight when the node is valid
					if (!toNode->getTile()->getVisible()) {
						weight = width;
					}
					graph->addWeightConnection(from, toNode->getLabel(), weight);
				}

				if (i > 0) {
					float weight = 0.0f;
					Ref<Node> toNode = wallList[i - 1][j];
					//Only set weight when the node is valid
					if (!toNode->getTile()->getVisible()) {
						weight = width;
					}
					graph->addWeightConnection(from, toNode->getLabel(), weight);
				}
			}
	
		}
	}

}


SceneActor::~SceneActor() {
	if (assetManager) assetManager->OnDestroy(), delete assetManager;
}

void SceneActor::OnDestroy() {
	Debug::Info("Deleting assets for SceneActor: ", __FILE__, __LINE__);
}


void SceneActor::Update(const float deltaTime) {

	for (std::pair<const char*, Ref<Actor>> actor : actorList) {
		actor.second->Update(deltaTime);
		//Set four walls
		//Plane p;
		//Ref<Actor> edge = GetComponent<Actor>("Edge");
		//Ref<CollisionComponent> edgeCollision = edge->GetComponent<CollisionComponent>();
		//Ref<Plane> p = std::dynamic_pointer_cast<Plane>(edgeCollision->getShape());
		
		//Check collision with four edges
		Ref<Plane> topEdge = std::make_shared<Plane>(0.0f, -1.0f, 0.0f, 8.5f);
		Ref<Plane> botEdge = std::make_shared<Plane>(0.0f, 1.0f, 0.0f, 8.5f);
		Ref<Plane> leftEdge = std::make_shared<Plane>(1.0f, 0.0f, 0.0f, 15.0f);
		Ref<Plane> rightEdge = std::make_shared<Plane>(-1.0f, 0.0f, 0.0f, 15.0f);
		std::vector<Ref<Plane>> edges;
		edges.push_back(topEdge);
		edges.push_back(botEdge);
		edges.push_back(leftEdge);
		edges.push_back(rightEdge);

		for (Ref<Plane> edge : edges) {
			if (PhysicsCollision::PlaneCollisionDetect(actor.second, edge)) {
				PhysicsCollision::PlaneSphereCollisionResponse(actor.second, edge);
			}
		}

		for (std::vector<Ref<Node>> wallRow : wallList) {
			for (Ref<Node> wall : wallRow) {
				wall->getTile()->Update(deltaTime);
			}
		}

		//Make collision tiles
		//Ref<Actor> wall = GetComponent <Actor>("Wall");
		//Ref<CollisionComponent> wallCollision = wall->GetComponent<CollisionComponent>();
		//Ref<Box> wallTile = std::dynamic_pointer_cast<Box>(wallCollision->getShape());
		//if (PhysicsCollision::BoxSphereCollision(actor.second, wallTile)) {

		//}

	}
}

void SceneActor::Render()const {

	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::pair<const char*, Ref<Actor>> actor : actorList) {
		actor.second->Render();
	}

	for (std::vector<Ref<Node>> wallRow : wallList) {
		for (Ref<Node> wall : wallRow) {
			wall->getTile()->Render();
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void SceneActor::HandleEvents(const SDL_Event& sdlEvent){
	for (std::pair<const char*, Ref<Actor>> actor : actorList) {
		actor.second->HandleEvents(sdlEvent);
	}

}

