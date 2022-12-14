#ifndef DIRECTIONCONDITION_H
#define DIRECTIONCONDITION_H

#include "Condition.h"
#include "VMath.h"
#include "Vector.h"

using namespace MATH;

class DirectionCondition : public Condition{
private:
	//angle in degree
	float minRange;
	float maxRange;

public:
	DirectionCondition(): Condition(){}
	DirectionCondition(Ref<Actor> owner_) : Condition(owner_) {}
	~DirectionCondition() {}
	virtual bool test();
	void setRange(float min, float max) {
		minRange = min;
		maxRange = max;
	}
};

#endif
