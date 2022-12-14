#ifndef ACTION_H
#define ACIION_H

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class Actor;

class Action{

protected:
	Ref<Actor> owner;

public:
	Action():owner(nullptr){}
	Action(Ref<Actor> owner_):owner(owner_){}
	~Action(){}
	virtual void Perform() = 0;
	void setOwner(Ref<Actor> owner_) { owner = owner_; }
};

#endif
