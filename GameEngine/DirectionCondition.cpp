#include "DirectionCondition.h"
#include "Actor.h"
#include "BodyComponent.h"
#include "Body.h"

bool DirectionCondition::test(){

    Ref<Body> body_ = owner->GetComponent<BodyComponent>()->getBody();

    float angle = body_->getRotation();

    Vec3 ownerDirection = body_->getVel();

    float magDirection = VMath::mag(ownerDirection);

    if (magDirection <= 0.01f) {
        return false;
    }
    else {
        ownerDirection = VMath::normalize(ownerDirection);
        //Calculate the angle, assuming the y axis is the origin
        Vec3 origin = Vec3(0.0f, 1.0f, 0.0f);
        float dot = VMath::dot(ownerDirection, origin);
        float denominator = VMath::mag(origin) * VMath::mag(ownerDirection);
        float value = dot / denominator;
        //std::cout << "value: " << value << "\n";
        angle = acos(value);
        //std::cout << "angleRadian: " << angle << "\n";
        angle *= RADIANS_TO_DEGREES;
        //std::cout << "angleDegree: " << angle << "\n";

    }
    //ownerDirection.print();


    //Map the angle into correct section
    //When using cos to find angle between two vectors, the range will be [0, 180]
    //which will cause anything greater than 180 not being accounted
    //When x is negative, it means that the angle is greater than 180
    if (ownerDirection.x < 0.0f) {
        angle = 360.0f - angle;
    }

    //If the max range is greater than 360.0f
    if (maxRange >= 360.0f && angle < minRange) {
        angle += 360.0f;

    }

    //Check if the direction is within the range given
    if (minRange <= angle && angle <= maxRange) {
        std::cout << "Min: " << minRange << " angle: " << angle << " Max: " << maxRange << "\n";
        return true;
    }

    return false;
}
