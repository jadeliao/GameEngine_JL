#ifndef SEEK_H
#define SEEK_H
#include "SteeringBehaviour.h"
class Seek : public SteeringBehaviour{

protected:
	Ref<Body> target;

public:
	Seek(Ref<Body> character_, Ref<Body> target_);
	virtual ~Seek();
	Ref<SteeringOutput> getSteering();

};

#endif

