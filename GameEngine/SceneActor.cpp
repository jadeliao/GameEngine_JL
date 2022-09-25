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

SceneActor::SceneActor(Ref<Component> parent_) : Actor(parent_) {
	assetManager = new AssetManager();
}

bool SceneActor::OnCreate_Scene() {
	if (isCreated) return isCreated;
	Debug::Info("Loading assets for SceneActor: ", __FILE__, __LINE__);
	for (std::pair<const char*, Ref<Actor>> actor : actorList) {
		if (actor.second->OnCreate() == false) {
			Debug::Error("Loading assets for SceneActor: ", __FILE__, __LINE__);
			isCreated = false;
		}
	}
	isCreated = true;
	return isCreated;
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
	}
}

void SceneActor::Render()const {

	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::pair<const char*, Ref<Actor>> actor : actorList) {
		actor.second->Render();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void SceneActor::HandleEvents(const SDL_Event& sdlEvent){
	for (std::pair<const char*, Ref<Actor>> actor : actorList) {
		actor.second->HandleEvents(sdlEvent);
	}
}

