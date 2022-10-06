#ifndef STEERING_BEHAVIOUR_H
#define STEERING_BEHAVIOUR_H

#include "VMath.h"
#include "KinematicBody.h"
#include "SteeringOutput.h"

class SteeringBehaviour {

protected:
	Ref<SteeringOutput> result;
	Ref<Body> character;

public:
	SteeringBehaviour();
	virtual Ref<SteeringOutput> getSteering() = 0;
	virtual ~SteeringBehaviour() {
		//if (result) delete result;
	}

};

#endif
