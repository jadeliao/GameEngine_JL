#include "AnimationComponent.h"
#include "MaterialComponent.h"
#include <iostream>

AnimationComponent::AnimationComponent(): Component(), currentAnimation(""), currentIndex(0), timePassed(0.0f) {}

AnimationComponent::AnimationComponent(Ref<Component> parent_): Component(parent_), currentAnimation(""), currentIndex(0), timePassed(0.0f) {}

AnimationComponent::~AnimationComponent(){
	OnDestroy();
}

bool AnimationComponent::OnCreate(){
	if (isCreated) return isCreated;
	isCreated = true;
	//Loop through all the texture, and create them
	for (auto animation : animationMap) {
		for (Ref<MaterialComponent> texture : animation.second) {
			if (!texture->OnCreate()) {
				isCreated = false;
				return isCreated;
			}
		}
	}
	return isCreated;
}

void AnimationComponent::OnDestroy(){
	for (auto animation : animationMap) {
		for (Ref<MaterialComponent> texture : animation.second) {
			texture->OnDestroy();
		}
	}
}

void AnimationComponent::Update(const float deltaTime_){

	timePassed += deltaTime_;

	std::vector<Ref<MaterialComponent>> currentList;

	//Find the current animation list
	for (auto animation : animationMap) {
		if (strcmp(currentAnimation, animation.first) == 0) {
			currentList = animation.second;
		}
	}

	if (timePassed > interval) {
		timePassed = 0.0f;

		if (currentList.empty()) return;

		//Move to next animation
		currentIndex++;

		//If it reaches the end of the animation, go back to the beginning animation
		if (currentIndex >= currentList.size()) {
			currentIndex = 0;
		}
	}

}

void AnimationComponent::Render() const{}

void AnimationComponent::AddAnimation(const char* name, std::vector<Ref<MaterialComponent>> textureList){
	animationMap[name] = textureList;

}

Ref<MaterialComponent> AnimationComponent::getAnimation(){

	std::vector<Ref<MaterialComponent>> currentList;

	//Find the current animation list
	for (auto animation : animationMap) {
		if (strcmp(currentAnimation, animation.first) == 0) {
			currentList = animation.second;
		}
	}

	if (!currentList.empty()) {

		return currentList[currentIndex];
	}

	return nullptr;
}

void AnimationComponent::setAnimation(const char* name_){
	currentAnimation = name_;
	currentIndex = 0;
}
