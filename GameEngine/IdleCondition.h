#ifndef IDLECONDITION_H
#define IDLECONDITION_H

#include "Condition.h"
#include "VMath.h"
#include "Vector.h"

using namespace MATH;

class IdleCondition : public Condition{
public:
	IdleCondition():Condition(){}
	IdleCondition(Ref<Actor> owner_) : Condition(owner_) {}
	~IdleCondition() {}
	virtual bool test();

};

#endif
