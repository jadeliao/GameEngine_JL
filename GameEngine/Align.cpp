#include "Align.h"


Align::Align(Ref<Body> character_, Ref<Body> target_) {
	character = character_;
	target = target_;
	targetRadius = 2.0f;
	slowRadius = 1.0f;
}

Align::~Align(){}

Ref<SteeringOutput> Align::getSteering() {
	float rotation = target->getOrientation() - character->getOrientation();
	//map to range to rotate with a smaller angle
	rotation = mapToRange(rotation);
	float rotationSize = abs(rotation);
	float targetRotation = target->getRotation();
	
	//What happens if target max rotation is 0? it will never rotate?
	if (rotationSize > slowRadius) {
		targetRotation = character->getMaxRotation();
	}
	else {
		targetRotation = character->getMaxRotation() * rotationSize / slowRadius;
	}
	if (rotationSize > 0.0f) {
		targetRotation *= rotation / rotationSize;
	}
	result->angular = targetRotation - character->getRotation();
	result->angular /= timeToTarget;


	return result;
}

double Align::mapToRange(double degree_){

	double r = fmod(degree_, 360.0);
	if (r < -180.0) {
		r += 360;
	}
	if (r >= 180.0) {
		r -= 360;
	}

	return r;
}