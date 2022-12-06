#pragma once
#include <unordered_map>
#include <vector>
#include <iostream>
#include "Actor.h"
#include "Matrix.h"
#include "Scene.h"


using namespace MATH;

template<typename T>
using Ref = std::shared_ptr<T>;

class CameraActor;
class LightActor;
class AssetManager;
class WallActor;
class Node;
class Graph;

class SceneActor : public Actor, public Scene {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	SceneActor(const SceneActor&) = delete;
	SceneActor(SceneActor&&) = delete;
	SceneActor& operator=(const SceneActor&) = delete;
	SceneActor& operator=(SceneActor&&) = delete;

protected:
	bool OnCreate_Scene();
	AssetManager* assetManager;
	Ref<Graph> graph;
	void graphConnection();
	std::vector<std::vector<Ref<Node>>> wallList;
	std::unordered_map<const char*, Ref<Actor>> actorList;
private:



public:
	SceneActor(Ref<Component> parent_);
	~SceneActor();
	void OnDestroy();
	bool OnCreate();
	void Update(const float deltaTime_);
	void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	template<typename ActorTemplate>
	void AddActor(const char* name_, Ref<ActorTemplate> actor_){
		if (std::dynamic_pointer_cast<Actor>(actor_)) {
			//Need to check memory for the string
			std::string *temp_name = new std::string();
			*temp_name = name_;
			actorList[temp_name->c_str()] = actor_;
		}
		else {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type to actor list - ignored\n";
#endif
		}
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent(const char* name_) const {
		Ref<Component> id;
		//Find only compares the address, therefore need to use the actual value to compare
		for (std::pair<const char*, Ref<Actor>> c : actorList) {
			std::string key = c.first;
			std::string givenkey = name_;
			if (key == givenkey) {
				id = c.second;
			}
		}
#ifdef _DEBUG
		if (!id) {
			std::cout << name_ << "\n";
			Debug::Error("Cannot find component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ComponentTemplate>(id);
	}

};

