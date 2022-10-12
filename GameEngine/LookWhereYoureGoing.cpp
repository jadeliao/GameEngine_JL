#include "LookWhereYoureGoing.h"

LookWhereYoureGoing::LookWhereYoureGoing(Ref<Body> character_, Ref<Body> target_): Align(character_, target_) {
}

LookWhereYoureGoing::~LookWhereYoureGoing(){
}

Ref<SteeringOutput> LookWhereYoureGoing::getSteering(){

	Vec3 velocity = character->getVel();
	float targetOri = target->getOrientation();
	if (VMath::mag(velocity) > VERY_SMALL) {
		target->setOrientation(atan2(-velocity.x, velocity.y));
	}

	result = Align::getSteering();
	target->setOrientation(targetOri);

	return result;
}
