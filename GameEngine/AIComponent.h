#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "MMath.h"
#include "VMath.h"
#include "Component.h"
#include "SteeringOutput.h"
#include "SteeringBehaviour.h"
#include "Actor.h"
#include "Path.h"
#include <iostream>
#include <vector>

using namespace MATH;
using namespace std;

enum SteeringType {
	seeking,
	aligning,
	looking,
	pathfollowing
};

class AIComponent : public Component {
private:
	Ref<SteeringOutput> steering;
	vector<SteeringType> steeringBehaviours;
	Ref<Actor> target;
	//std::string targetName;
	Ref<Path> path;

public:
	AIComponent(Ref<Component> parent_, Ref<Actor> target_);
	~AIComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render()const;
	void getSteeringOutputs(Ref<Body> self, SteeringType steeringType_);
	void addSteeringBehaviour(SteeringType steeringType_);

	void setTarget(Ref<Actor> target_);
	//void setTarget(std::string targetName_) { targetName = targetName_; }
	Ref<Actor> getTarget() { return target; }
	Ref<SteeringOutput> getSteering() { return steering; }
	void setPath(Ref<Path> path_) { path = path_; }
};


#endif