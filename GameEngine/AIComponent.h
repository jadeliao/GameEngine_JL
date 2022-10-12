#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "MMath.h"
#include "VMath.h"
#include "Component.h"
#include "SteeringOutput.h"
#include "SteeringBehaviour.h"
#include "Actor.h"

using namespace MATH;
using namespace std;

enum SteeringType {
	seeking,
	aligning,
	looking
};

class AIComponent : public Component {
private:
	Ref<SteeringOutput> steering;
	SteeringType steeringType;
	Ref<Actor> target;
	//Ref<SteeringBehaviour> steering_algorithm;

public:
	AIComponent(Ref<Component> parent_, Ref<Actor> target_);
	~AIComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Update(const float deltaTime_, Ref<Body> self_, SteeringType steeringType_);
	void Render()const;
	void addSteeringBehaviour(Ref<Body> self, SteeringType steeringType_);

	void setTarget(Ref<Actor> target_) { target = target_; }
	Ref<Actor> getTarget() { return target; }
	Ref<SteeringOutput> getSteering() { return steering; }

};


#endif