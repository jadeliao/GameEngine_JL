#ifndef FOLLOWPATH_H
#define FOLLOWPATH_H

#include "Arrive.h"
#include "Path.h"

template<typename T>
using Ref = std::shared_ptr<T>;

class FollowPath : public Arrive{
protected:
	Ref<Path> path;
	Vec3 targetPos;

public:
	FollowPath(Ref<Body> character_, Ref<Body> target_, Ref<Path> path_);
	~FollowPath();
	Ref<SteeringOutput> getSteering() override;
	void setPath(Ref<Path> path_) { path = path_; }
};

#endif

