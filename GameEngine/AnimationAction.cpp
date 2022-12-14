#include "AnimationAction.h"
#include "AnimationComponent.h"
#include "Actor.h"

void AnimationAction::Perform(){
	Ref<AnimationComponent> animation_ = owner->GetComponent<AnimationComponent>();
	animation_->setAnimation(animationName);
	std::cout << animationName << "\n";

}
