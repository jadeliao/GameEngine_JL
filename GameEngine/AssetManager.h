#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include "Component.h"
#include "Actor.h"
#include "WallActor.h"
#include "Debug.h"
#include "tinyxml2.h"
#include "Node.h"

class AIComponent;
class StateMachine;
class State;
class Action;
class Transition;
class Condition;

using namespace tinyxml2;


struct StateTransition {
	Ref<State> state;
	std::vector<const char*> transitionList;
};

struct StateMachineAsset {
	const char* initialState;
	Ref<StateMachine> stateMachine;
	std::unordered_map<const char*, Ref<StateTransition>> stateList;
	std::unordered_map<const char*, Ref<Action>> actionList;
	std::unordered_map<const char*, Ref<Transition>> transitionList;
	std::unordered_map<const char*, Ref<Condition>> conditionList;

	Ref<Action> getAction(const char* name_) {
		for (auto action_ : actionList) {
			if (strcmp(name_, action_.first) == 0) {
				return action_.second;
			}
		}
		return nullptr;
	}

	Ref<State> getState(const char* name_) {
		for (auto state_ : stateList) {
			if (strcmp(name_, state_.first) == 0) {
				return state_.second->state;
			}
		}
		return nullptr;
	}

	Ref<Condition> getCondition(const char* name_) {
		for (auto condition_ : conditionList) {
			if (strcmp(name_, condition_.first) == 0) {
				return condition_.second;
			}
		}
		return nullptr;
	}

	Ref<Transition> getTransition(const char* name_) {
		for (auto transition_ : transitionList) {
			if (strcmp(name_, transition_.first) == 0) {
				return transition_.second;
			}
		}
		return nullptr;
	}

};


class AssetManager{
private:
	std::unordered_map<const char*, Ref<Actor>> actorList;
	std::unordered_map<const char*, Ref<Component>> componentCatalog;
	std::unordered_map<Ref<AIComponent>, const char*> aiList;

	std::vector<std::vector<Ref<Node>>> wallList; //double vector for walls

	std::unordered_map <const char*, Ref<StateMachineAsset>> stateMachineList;

	Ref<WallActor> wallActor;
	const char* scene;
	void ReadManiFest();
	XMLDocument doc;

	void AddComponentData(XMLElement* componentData);
	void AddActorData(XMLElement* actorData);
	void AddStateMachine(XMLElement* stateMachineData);
	void AddState(XMLElement* stateData);
	void AddAction(XMLElement* actionData);
	void AddTransition(XMLElement* stateData);
	void AddCondtion(XMLElement* stateData);
	void AddWallData(XMLElement* wallData);
	void SetAITarget();

public:
	AssetManager();
	AssetManager(const char* scene_);
	~AssetManager();
	bool OnCreate();
	void OnDestroy();
	std::unordered_map<const char*, Ref<Actor>> getActors() { return actorList; }
	std::vector<std::vector<Ref<Node>>> getWalls() { return wallList; }
	void RemoveAllComponents();

	template<typename ActorTemplate>
	void AddActor(const char* name_, Ref<ActorTemplate> actor_) {
		if (std::dynamic_pointer_cast<Actor>(actor_)) {
			//Need to check memory for the string
			std::string* temp_name = new std::string();
			*temp_name = name_;
			actorList[temp_name->c_str()] = actor_;
		}
		else {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type to actor list - ignored\n";
#endif
		}
	}

	Ref<Actor> GetActor(const char* name_) const {
		Ref<Actor> id;
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
			Debug::Error("Cannot find actor", __FILE__, __LINE__);
			return Ref<Actor>(nullptr);
		}
#endif
		return id;
	}


	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name_, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		componentCatalog[name_] = t;
	}

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(const char* name_, Ref<ComponentTemplate> component_) {
		componentCatalog[name_] = component_;
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent(const char* name_) const {
		Ref<Component> id;
		//Find only compares the address, therefore need to use the actual value to compare
		for (std::pair<const char*, Ref<Component>> c : componentCatalog) {
			std::string key = c.first;
			std::string givenkey = name_;
			if (key == givenkey) {
				id = c.second;
			}
		}
#ifdef _DEBUG
		if (!id) {
			Debug::Error("Cannot find component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ComponentTemplate>(id);
	}



};

