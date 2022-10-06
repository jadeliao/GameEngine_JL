#include "SteeringBehaviour.h"

SteeringBehaviour::SteeringBehaviour() {
	result = std::make_shared<SteeringOutput>();
	character = nullptr;
}
