#include "Arrive.h"

Arrive::Arrive(Ref<Body> character_, Ref<Body> target_){
	character = character_;
	target = target_;
	targetRadius = 2.0f;
	slowRadius = 0.1f;
}

Arrive::~Arrive(){}

Ref<SteeringOutput> Arrive::getSteering(){
	// Get direction to the target	
	result->linear = target->getPos() - character->getPos();
	Vec3 direction = VMath::normalize(result->linear);
	float distance = VMath::mag(result->linear);
	float targetSpeed = VMath::mag(target->getVel());

	if (distance > slowRadius) {
		targetSpeed = character->getMaxSpeed();
	}
	else {
		targetSpeed = character->getMaxSpeed() * distance / slowRadius;
	}

	//Find target velocity
	Vec3 targetVelocity = direction * targetSpeed;
	result->linear = targetVelocity - character->getVel();
	result->linear /= timeToTarget;

	//Clip to max acceleration
	if (VMath::mag(result->linear) > character->getMaxAcceleration()) {
		result->linear = VMath::normalize(result->linear);
		result->linear *= character->getMaxAcceleration();
	}

	result->angular = 0.0f;

	return result;
}
