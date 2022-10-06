#ifndef BODY_H
#define BODY_H

#include "MMath.h"
#include "VMath.h"
#include <SDL.h>
#include <SDL_image.h>

using namespace MATH;

template<typename T>
using Ref = std::shared_ptr<T>;

class Body {
protected:
	// inherited classes can access this
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float mass;
    float orientation;		// facing this direction
    float rotation;			// rotating at this speed (radians per second?)
    float angular;          // angular acceleration
    float maxSpeed;
    float maxAcceleration;
    float maxRotation;
    float maxAngular;
    float radius;           // for getting near walls

public:
    Body();
	Body(
        Vec3 pos_, Vec3 vel_, Vec3 accel_, 
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        float maxSpeed_,
        float maxAcceleration_,
        float maxRotation_,
        float maxAngular_
    );
	virtual ~Body();
    virtual void Update(float deltaTime);
    virtual void ApplyForce(Vec3 force_);
    virtual Vec3 getPos() { return pos; }
    virtual Vec3 getVel() { return vel; }
    virtual Vec3 getAccel() { return accel; }
    virtual float getMass() { return mass; }
    virtual float getOrientation() { return orientation; }
    virtual float getRotation() { return rotation; }
    virtual float getAngular() { return angular; }
    virtual float getMaxSpeed() { return maxSpeed; }
    virtual float getMaxAcceleration() { return maxAcceleration; }
    virtual float getMaxRotation() { return maxRotation; }
    virtual float getMaxAngular() { return maxAngular; }

    virtual void HandleEvents( const SDL_Event& event );

    virtual void setPos(Vec3 pos_) { pos = pos_; }
    virtual void setVel(Vec3 vel_) { vel = vel_; }
    virtual void setAccel(Vec3 accel_) { accel = accel_; }

	// Added this as public to deal with my demo 
	// so that mouse position can be copied into a Body.
    //virtual void setPos( Vec3 pos );
};

#endif /* BODY_H */
