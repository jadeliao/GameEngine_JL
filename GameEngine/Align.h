#ifndef ALIGN_H
#define ALIGN_H

#include "SteeringBehaviour.h"
using namespace std;

class Align : public SteeringBehaviour {

protected:
	Ref<Body> target;
	float targetRadius;
	float slowRadius;
	float timeToTarget = 0.1f;
	double mapToRange(double degree_);

public:
	Align(Ref<Body> character_, Ref<Body> target_);
	virtual ~Align();
	Ref<SteeringOutput> getSteering();
};

#endif