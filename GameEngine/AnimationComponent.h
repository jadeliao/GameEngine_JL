#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include "Component.h"
#include <unordered_map>
#include <vector>

class MaterialComponent;

class AnimationComponent : public Component{

private:
	std::unordered_map<const char*, std::vector<Ref<MaterialComponent>>> animationMap;
	const char* currentAnimation;
	int currentIndex;
	float interval = 0.5f;
	float timePassed;
public:
	AnimationComponent();
	AnimationComponent(Ref<Component> parent_);
	virtual ~AnimationComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
	void AddAnimation(const char* name, std::vector<Ref<MaterialComponent>> textureList);
	Ref<MaterialComponent> getAnimation();
	void setAnimation(const char* name_);
};

#endif