#include "IdleCondition.h"
#include "Actor.h"
#include "BodyComponent.h"
#include "Body.h"

bool IdleCondition::test(){

    Vec3 ownerDirection = owner->GetComponent<BodyComponent>()->getBody()->getVel();

    if (VMath::mag(ownerDirection) <= 0.01f) {
        return true;
    }
    return false;
}
