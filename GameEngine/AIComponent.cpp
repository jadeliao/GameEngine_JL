#include "AIComponent.h"
#include "BodyComponent.h"
#include "Seek.h"
#include "Align.h"
#include "LookWhereYoureGoing.h"


AIComponent::AIComponent(Ref<Component> parent_, Ref<Actor> target_): 
	Component(parent_), target(target_), targetName("none") {
	steering = std::make_shared<SteeringOutput>();
}

AIComponent::~AIComponent() {

}

bool AIComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}

void AIComponent::OnDestroy() {}

void AIComponent::Update(const float deltaTime_) {}

void AIComponent::Render() const {}

void AIComponent::getSteeringOutputs(Ref<Body> self_, SteeringType steeringType_){
	vector<Ref<SteeringOutput>> steering_outputs;
	//Only perform action if there exists a target
	Ref<BodyComponent> targetBody = target->GetComponent<BodyComponent>();

	if (targetBody) {
		//SteeringType steeringBehav;
		for (SteeringType steeringType : steeringBehaviours) {
			SteeringBehaviour* steering_algorithm = nullptr;
			switch (steeringType) {
			case seeking:
				steering_algorithm = new Seek(self_, targetBody->getBody());
				break;
			case aligning:
				steering_algorithm = new Align(self_, targetBody->getBody());
				break;
			case looking:
				steering_algorithm = new LookWhereYoureGoing(self_, targetBody->getBody());
				break;
			default:
				break;
			}
			//Add the output to vector
			steering_outputs.push_back(steering_algorithm->getSteering());
			//Reset steering
			steering->linear = Vec3();
			steering->angular = 0.0f;
			//Combines the steerings
			for (unsigned i = 0; i < steering_outputs.size(); i++) {
				if (steering_outputs[i]) {
					//Add steering outputs to the steering reference (pass in the address)
					*steering += *steering_outputs[i];
				}
			}
			if (steering_algorithm) delete steering_algorithm;
		}
	}
	else {
		cerr << "No target found\n";
	}

}

void AIComponent::addSteeringBehaviour(SteeringType steeringType_) {
	steeringBehaviours.push_back(steeringType_);
}


void AIComponent::setTarget(Ref<Actor>target_) {
	target = target_;
}

