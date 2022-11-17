#ifndef PHYSICSCOLLISION_H
#define PHYSICSCOLLISION_H

#include "VMath.h"
#include "Plane.h"
#include "Box.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "BodyComponent.h"

using namespace MATH;
using namespace GEOMETRY;

class PhysicsCollision {

private:
	static bool PlaneSphereCollision(const Ref<CollisionComponent> &collisionShape_, const Ref<Plane>& p);

public:
	static bool PlaneCollisionDetect(Ref<Actor> &actor_, const Ref<Plane>&plane_);
	static void PlaneSphereCollisionResponse(Ref<Actor>& actor_, const Ref<Plane>& p);
	static bool BoxSphereCollision(Ref<Actor>& actor_, const Ref<Box>& box_);
};

#endif

