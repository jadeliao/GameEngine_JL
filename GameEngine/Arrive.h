#ifndef ARRIVE_H
#define ARRIVE_H

#include "SteeringBehaviour.h"
class Arrive : public SteeringBehaviour{
protected:
	Ref<Body> target;
	float targetRadius;
	float slowRadius;
	float timeToTarget = 0.1f;
public:
	Arrive(Ref<Body> character_, Ref<Body> target_);
	~Arrive();
	Ref<SteeringOutput> getSteering();
};


#endif

