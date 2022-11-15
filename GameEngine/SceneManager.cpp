#include <SDL.h>
#include "SceneManager.h"
#include "UserInterface.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Timer.h"
#include "Window.h"
#include "Scene2.h"
#include "DemoSceneUI.h"
#include "DemoScene.h"

SceneManager::SceneManager(): 
	currentScene(nullptr), window(nullptr), timer(nullptr),
	fps(60), isRunning(false), fullScreen(false), currentUI(nullptr) {
	Debug::Info("Starting the SceneManager", __FILE__, __LINE__);
}

SceneManager::~SceneManager() {
	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}

	if (currentUI) {
		currentUI->OnDestroy();
		delete currentUI;
		currentUI = nullptr;
	}
	Debug::Info("Deleting the SceneManager", __FILE__, __LINE__);
}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}

	/********************************   Default first scene   ***********************/
	BuildNewScene(SCENE_NUMBER::DemoScene);


	return true;
}

/// This is the whole game
void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		timer->UpdateFrameTicks();
		HandleEvents();
		currentScene->Update(timer->GetDeltaTime());
		currentUI->Update(timer->GetDeltaTime());
		currentScene->Render();
		currentUI->Render();
		SDL_GL_SwapWindow(window->getWindow());
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void SceneManager::HandleEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				isRunning = false;
				return;
				[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings
			default:
				break;
			}
		}
		if (currentScene == nullptr) {
			Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

void SceneManager::BuildNewScene(SCENE_NUMBER scene) {
	bool status; 

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	if (currentUI != nullptr) {
		currentUI->OnDestroy();
		delete currentUI;
		currentUI = nullptr;
	}

	switch (scene) {
	case SCENE_NUMBER::SCENE2:
		currentScene = new Scene2();
		status = currentScene->OnCreate();
		break;

	case SCENE_NUMBER::DemoScene:
		currentScene = new DemoScene();
		status = currentScene->OnCreate();
		currentUI = new DemoSceneUI();
		status = currentUI->OnCreate(window);
		break;

	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}	
}


