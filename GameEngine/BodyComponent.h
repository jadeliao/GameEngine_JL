#ifndef BODYCOMPONENT_H
#define BODYCOMPONENT_H

#include "MMath.h"
#include "VMath.h"
#include "Component.h"
#include "Body.h"

using namespace MATH;

enum BodyType {
    Base = 0,
    Kinematic
};



class BodyComponent : public Component {
private:
    Ref<Body> body;
    BodyType bodyType;
    void setTransform();

public:
    BodyComponent(Ref<Component> parent_, BodyType bodyType_);
    BodyComponent(Ref<Component> parent_, BodyType bodyType_, 
        Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        float maxSpeed_,
        float maxAcceleration_,
        float maxRotation_,
        float maxAngular_);
	virtual ~BodyComponent();
    bool OnCreate();
    void OnDestroy();
    virtual void Update(const float deltaTime);
    void Render() const;
    virtual void HandleEvents( const SDL_Event& event );
   
    BodyType getType() { return bodyType; }
    void setPos(Vec3 pos_) { body->setPos(pos_); }
    void setVel(Vec3 vel_) { body->setVel(vel_); }
    void setAccel(Vec3 accel_) { body->setAccel(accel_); }
    void setOrientation(float orientation_) { body->setOrientation(orientation_); }
    Ref<Body> getBody() { return body; }

    void print();
};

#endif /* BODY_H */
