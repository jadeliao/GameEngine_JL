#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include "Component.h"
#include "Debug.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class AssetManager{
private:

	std::unordered_map<const char*, Ref<Component>> componentCatalog;
	void ReadManiFest();
	XMLDocument doc;

public:
	AssetManager();
	~AssetManager();
	bool OnCreate();
	void OnDestroy();

	void RemoveAllComponents();

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

