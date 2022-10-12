#include "BodyComponent.h"
#include "KinematicBody.h"
#include "AIComponent.h"
#include "Actor.h"
#include "TransformComponent.h"

BodyComponent::BodyComponent(Ref<Component> parent_, BodyType bodyType_) : Component(parent_), bodyType(bodyType_) {
	switch (bodyType_) {
	case Kinematic:
		body = std::make_shared<KinematicBody>();
		break;
	default:
		body = std::make_shared<Body>();
		break;
	}
}

BodyComponent::BodyComponent(Ref<Component> parent_, BodyType bodyType_,
	Vec3 pos_, Vec3 vel_, Vec3 accel_, 
	float mass_, float radius_, 
	float orientation_, float rotation_, float angular_, 
	float maxSpeed_, float maxAcceleration_, float maxRotation_, float maxAngular_): bodyType(bodyType_) {

	switch (bodyType_) {
	case Kinematic:
		body = std::make_shared<KinematicBody>(pos_, vel_, accel_,
			mass_, radius_,
			orientation_, rotation_, angular_,
			maxSpeed_, maxAcceleration_, maxRotation_, maxAngular_);
		break;
	default:
		body = std::make_shared<Body>(pos_, vel_, accel_,
			mass_, radius_,
			orientation_, rotation_, angular_,
			maxSpeed_, maxAcceleration_, maxRotation_, maxAngular_);
		break;
	}

}

BodyComponent::~BodyComponent(){
    OnDestroy();
}

bool BodyComponent::OnCreate(){
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}

void BodyComponent::OnDestroy(){
	isCreated = false;
}


void BodyComponent::Update(float deltaTime){
	float oldOrientation = body->getOrientation();
	Ref<Actor> parentActor = std::dynamic_pointer_cast<Actor>(GetParent());
	if (parentActor) {
		Ref<AIComponent> AIComp = parentActor->GetComponent<AIComponent>();
		switch (bodyType) {
		case Kinematic:
			if (AIComp) {
				AIComp->Update(deltaTime, body, aligning);
				//AIComp->Update(deltaTime, body, aligning);
				std::dynamic_pointer_cast<KinematicBody>(body)->Update(deltaTime, AIComp->getSteering());
			}
			else {
				body->Update(deltaTime);
			}
			break;
		default:
			body->Update(deltaTime);
			break;
		}
		Ref<TransformComponent> transform = parentActor->GetComponent<TransformComponent>();
		Vec3 newPos = body->getPos();
		float newOrientation = body->getOrientation(); //
		if (oldOrientation != newOrientation) {
			transform->SetTransform(newPos,
				transform->GetQuaternion() * QMath::angleAxisRotation(newOrientation * 180.0f / M_PI,
					Vec3(0.0f, 0.0f, 1.0f)), transform->GetScale());
		}
		else {
			transform->SetTransform(newPos, transform->GetQuaternion(), transform->GetScale());
		}

	}

}

void BodyComponent::Render() const{}

void BodyComponent::HandleEvents( const SDL_Event& event ){
    // etc
}

void BodyComponent::print(){
	body->print();
}

