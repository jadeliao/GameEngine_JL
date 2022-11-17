#ifndef FOLLOWPATH_H
#define FOLLOWPATH_H
#include "Arrive.h"

class FollowPath : public Arrive{
protected:

public:
	FollowPath(Ref<Body> character_, Ref<Body> target_);
	~FollowPath();
	Ref<SteeringOutput> getSteering() override;
};

#endif

