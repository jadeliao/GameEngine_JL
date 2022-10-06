#ifndef KINEMATICSTEERINGOUTPUT_H
#define KINEMATICSTEERINGOUTPUT_H

#include "Vector.h"
#include "VMath.h"

using namespace MATH;

class KinematicSteeringOutput{

private:


public:
	Vec3 velocity;
	float rotation;
	KinematicSteeringOutput();
	KinematicSteeringOutput(Vec3 velocity_, float rotation_);
	~KinematicSteeringOutput();

};

#endif
