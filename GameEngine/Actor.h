#pragma once
#include <vector> 
#include <iostream>
#include "Component.h"
#include "Matrix.h"

using namespace MATH;

union SDL_Event;
class StateMachine;

class Actor: public Component {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	//Actor(const Actor&) = delete;
	Actor(Actor&&) = delete;
	Actor& operator=(const Actor&) = delete;
	Actor& operator=(Actor&&) = delete;

protected:
	std::vector <Ref<Component>> components;
	Ref<StateMachine> stateMachine;
private:
	Matrix4 modelMatrix;

public:
	Actor(Ref<Component> parent_);
	Actor(const Actor& actor_);
	~Actor();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);
	void setStateMachine(Ref<StateMachine> stateMachine_) { stateMachine = stateMachine_; }
	/// Footnote to those who think you can't write code in the header file - this is true
	/// with a few exceptions. (1) You can't inline code (implicitly or not) unless it is in 
	/// the header file and (2) templates must be in the header file
	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(Args&& ... args_) {
		/// before you add the component, ask if you have the component in the list already,
		/// if so - don't add a second one. 
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		/// Using std::make_shared to do the work. This is the new idea!,
		components.push_back(std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...));
	}


	template<typename ComponentTemplate>
	void AddComponent(Ref<ComponentTemplate> component_) {
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		components.push_back(component_);
	}

	template<typename ComponentTemplate>
	Ref<ComponentTemplate> GetComponent() const {
		for (auto component : components) {
			if (dynamic_cast<ComponentTemplate*>(component.get())) {
				/// This is a dynamic cast designed for shared_ptr's
				/// en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
				return std::dynamic_pointer_cast<ComponentTemplate>(component);
			}
		}
		return Ref<ComponentTemplate>(nullptr);
	}

	template<typename ComponentTemplate>
	void RemoveComponent() {
		for (unsigned int i = 0; i < components.size(); i++) {
			if (dynamic_cast<ComponentTemplate*>(components[i].get()) != nullptr) {
				components[i]->OnDestroy();
				components[i].reset();
				components[i] = nullptr;
				///This removes the component from the vector list
				components.erase(components.begin() + i);
				break;
			}
		}
	}

	std::vector<Ref<Component>> GetComponentsList() const { return components; }

	void RemoveAllComponents();

	void ListComponents() const;

	Matrix4 GetModelMatrix();
	

};

