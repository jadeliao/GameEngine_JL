#include "KinematicSeek.h"

KinematicSeek::KinematicSeek(Ref<Body> character_, Ref<Body> target_):
	character(character_), target(target_) {

	result = std::make_shared<KinematicSteeringOutput>();
}

KinematicSeek::~KinematicSeek(){
	//if (result) delete result;
}

Ref<KinematicSteeringOutput> KinematicSeek::getSteering()
{

	result->velocity = target->getPos() - character->getPos();

	result->velocity = VMath::normalize(result->velocity);
	result->velocity = character->getMaxSpeed() * result->velocity;
	//result->velocity *= character->getMaxAcceleration();
	result->rotation = 0.0f;

	return result;
}
