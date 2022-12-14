#ifndef ANIMATIONACTION_H
#define ANIMATIONACTION_H
#include "Action.h"

class AnimationAction : public Action{
private:
	const char* animationName;

public:
	AnimationAction(Ref<Actor> owner_) :Action(owner_), animationName("") {}
	~AnimationAction(){}
	virtual void Perform();
	void setAnimationName(const char* name_) { animationName = name_; }

};

#endif