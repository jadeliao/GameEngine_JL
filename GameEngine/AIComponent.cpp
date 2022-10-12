#include "AIComponent.h"
#include "BodyComponent.h"
#include "Seek.h"
#include "Align.h"
#include "LookWhereYoureGoing.h"
#include <vector>

AIComponent::AIComponent(Ref<Component> parent_, Ref<Actor> target_): 
	Component(parent_), target(target_) {
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

void AIComponent::Update(const float deltaTime_, Ref<Body> self_, SteeringType steeringType_) {

	SteeringOutput* steering = new SteeringOutput();

	addSteeringBehaviour(self_, steeringType_);

	//Preform different steering according to the steering type


	if (steering) delete steering;

}

void AIComponent::Render() const {

}

void AIComponent::addSteeringBehaviour(Ref<Body> self_, SteeringType steeringType_){
	vector<Ref<SteeringOutput>> steering_outputs;
	//Only perform action if there exists a target
	Ref<BodyComponent> targetBody = target->GetComponent<BodyComponent>();
	if (targetBody) {
		Ref<SteeringBehaviour> steering_algorithm;
		switch (steeringType_) {
		case seeking:
			steering_algorithm = std::make_shared<Seek>(self_, targetBody->getBody());
			break;
		case aligning:
			steering_algorithm = std::make_shared<Align>(self_, targetBody->getBody());
			break;
		case looking:
			steering_algorithm = std::make_shared<LookWhereYoureGoing>(self_, targetBody->getBody());
			break;
		default:
			break;
		}
		steering_outputs.push_back(steering_algorithm->getSteering());
		steering->linear = Vec3();
		steering->angular = 0.0f;
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


