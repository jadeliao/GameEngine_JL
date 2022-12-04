#include "FollowPath.h"

FollowPath::FollowPath(Ref<Body> character_, Ref<Body> target_, Ref<Path> path_): Arrive(character_, target_), path(path_) {

    //Make a new body target for follow path
    target = std::make_shared<Body>();
    targetRadius = 2.5f;
    slowRadius = 2.0f;

}

FollowPath::~FollowPath(){

}

Ref<SteeringOutput> FollowPath::getSteering(){

    Vec3 target_position;

    if (!path) return nullptr;

    target_position = path->getCurrentNode()->getPos();

    if (VMath::distance(character->getPos(), target_position) <= slowRadius) {
        //Incremented for next steering request
        path->incrementCurrentNode(1);
    }

    target->setPos(target_position);

    return Arrive::getSteering();
}
