#include "KinematicBody.h"

void KinematicBody::Update( float deltaTime, Ref<SteeringOutput> steering)
{
    // Update position, call Update from parent class
    // Note that would update velocity too, but the acceleration in the member object is 0.0!
    Body::Update( deltaTime );

    // Adjust velocity and rotation according to steering input
    if ( steering )
    {
        accel = steering->linear;
        angular = steering->angular ;
    }

    if (VMath::mag(accel) > maxAcceleration) {
        accel = VMath::normalize(accel) * maxAcceleration;
    }

    //if (angular > maxAngular) {
    //    angular = maxAngular;
    //}

    angular = angular > maxAngular ? maxAngular : angular;

}
