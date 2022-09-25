#pragma once
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

union SDL_Event;

class Component {
	public:
		Component():parent(nullptr), isCreated(false) {}
		Component(Ref<Component> parent_):parent(parent_), isCreated(false)  {}
		virtual ~Component() = default;
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Update(const float deltaTime_) = 0;
		virtual void Render()const = 0;
	protected:
		Ref<Component> parent;
		bool isCreated;
		Ref<Component> GetParent() const { return parent; }
};

