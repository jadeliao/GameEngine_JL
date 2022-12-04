#include "Align.h"


Align::Align(Ref<Body> character_, Ref<Body> target_) {
	character = character_;
	target = target_;
	targetRadius = 2.0f;
	slowRadius = 0.1f;
}

Align::~Align(){}

Ref<SteeringOutput> Align::getSteering() {
	float rotation = target->getOrientation() - character->getOrientation();
	//cout << "targetOri " << target->getOrientation() << endl;
	//cout << "charcterOri " << character->getOrientation() << endl;
	//map to range to rotate with a smaller angle
	rotation = mapToRange(rotation);
	float rotationSize = abs(rotation);
	float targetRotation = target->getRotation();

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

float Align::mapToRange(float degree_){

	degree_ *= 180.0f / M_PI;
	float r = fmod(degree_, 360.0f);
	if (r < -180.0f) {
		r += 360.0f;
	}
	if (r >= 180.0f) {
		r -= 360.0f;
	}

	r *= M_PI / 180.0f;
	return r;
}