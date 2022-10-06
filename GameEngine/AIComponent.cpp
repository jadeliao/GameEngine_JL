#include "AIComponent.h"
#include "Seek.h"
#include "BodyComponent.h"
#include <vector>

AIComponent::AIComponent(Ref<Component> parent_, SteeringType steeringType_, Ref<Actor> target_): 
	Component(parent_), steeringType(steeringType_), target(target_) {
	steering = std::make_shared<SteeringOutput>();
}

AIComponent::~AIComponent() {

}

bool AIComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}

void AIComponent::OnDestroy() {

}

void AIComponent::Update(const float deltaTime_) {

}

void AIComponent::Update(const float deltaTime_, Ref<Body> self_) {

	Ref<SteeringOutput> steering = std::make_shared<SteeringOutput>();
	switch (steeringType) {
	case seek:
		seekTarget(self_);
		break;
	default:
		break;
	}

}

void AIComponent::Render() const {

}

void AIComponent::seekTarget(Ref<Body> self_){
	vector<Ref<SteeringOutput>> steering_outputs;
	//Only do seek if there exists a target
	Ref<BodyComponent> targetBody = target->GetComponent<BodyComponent>();
	if (targetBody) {
		Ref<SteeringBehaviour> steering_algorithm = std::make_shared<Seek>(self_, targetBody->getBody());
		steering_outputs.push_back(steering_algorithm->getSteering());

		for (unsigned i = 0; i < steering_outputs.size(); i++) {
			if (steering_outputs[i]) {
				//Add steering outputs to the steering reference (pass in the address)
				*steering += *steering_outputs[i];
			}
		}
	}
	else {
		cerr << "No target found\n";
	}

}

