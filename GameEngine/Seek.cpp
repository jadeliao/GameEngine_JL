#include "Seek.h"

Seek::Seek(Ref<Body> character_, Ref<Body> target_) {
	character = character_;
	target = target_;
}

Seek::~Seek() {


}

Ref<SteeringOutput> Seek::getSteering() {

	// Get direction to the target
	result->linear = target->getPos() - character->getPos();

	// accelerate along this direction
	result->linear = VMath::normalize(result->linear) * character->getMaxAcceleration();

	result->angular = 0.0f;

	return result;
}

