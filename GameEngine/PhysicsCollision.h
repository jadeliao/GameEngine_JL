#ifndef PHYSICSCOLLISION_H
#define PHYSICSCOLLISION_H

#include "BodyComponent.h"
#include "Body.h"

class PhysicsCollision {
public:
	static bool CollisionDetect(Body &object1, Body &object2);
	
};

#endif

