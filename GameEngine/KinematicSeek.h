#ifndef KINEMATICSEEK_H
#define KINEMATICSEEK_H

#include "Vector.h"
#include "VMath.h"
#include "Body.h"
#include "KinematicSteeringOutput.h"

using namespace MATH;

class KinematicSeek {
private:
	Ref<KinematicSteeringOutput> result;
	Ref<Body> character;
	Ref<Body> target;


public:
	KinematicSeek(Ref<Body> character_, Ref<Body> target_);
	~KinematicSeek();
	Ref<KinematicSteeringOutput> getSteering();


};

#endif