#ifndef CONDITION_H
#define CONDITION_H

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class Actor;

class Condition{
protected:
	Ref<Actor> owner;

public:
	Condition(): owner(nullptr){}
	Condition(Ref<Actor> owner_): owner(owner_){}
	~Condition(){}
	virtual bool test() = 0;

};

#endif
