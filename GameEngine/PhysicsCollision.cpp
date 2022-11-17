#include "PhysicsCollision.h"
#include "Body.h"
#include "Sphere.h"
#include "Shape.h"

bool PhysicsCollision::PlaneCollisionDetect(Ref<Actor> &actor_, const Ref<Plane>& plane_){
	bool status = false;
	Ref<BodyComponent> actorBody = actor_->GetComponent<BodyComponent>();
	Ref<CollisionComponent> actorCollision = actor_->GetComponent<CollisionComponent>();
	//Check if both componenets exist
	if (actorBody && actorCollision) {
		//Get shape type to decided on collision type
		ShapeType actorShape = actorCollision->getShapeType();
		//Perform sphere collision
		if (actorShape == ShapeType::sphere) {
			status = PlaneSphereCollision(actorCollision, plane_);
			if (status) {


			}
		}
		else if (actorShape == ShapeType::cylinder) {

		}
		else if (actorShape == ShapeType::capsule) {

		}
		else if (actorShape == ShapeType::box) {

		}
		actorBody->setCollide(status);
	}

	return status;
}

bool PhysicsCollision::PlaneSphereCollision(const Ref<CollisionComponent>& collisionShape_, const Ref<Plane>& p){
	Ref<Sphere> sphere_ = std::dynamic_pointer_cast<Sphere>(collisionShape_->getShape());
	//Find distance between the position of the object and the plane
	
	float distance = VMath::dot(sphere_->getPos(), *p) + p->d - sphere_->r;
	// if d < 0 the plane is behind the object, and collision never happens
	if (0.0f < distance < VERY_SMALL) return true;
	return false;
}

void PhysicsCollision::PlaneSphereCollisionResponse(Ref<Actor>& actor_, const Ref<Plane>& plane_){
	Ref<Body> body = actor_->GetComponent<BodyComponent>()->getBody();
	Vec3 vel = body->getVel();
	//if vel is 0, give it a small value to perform response
	if (VMath::mag(vel) <= VERY_SMALL) {
		Vec3 p = VMath::normalize(*plane_);
		body->setPos(body->getPos() + p * 0.1f);
	}
	else {
		Vec3 bodyVel = body->getVel();
		//Find projection vector
		Vec3 projectionVec = VMath::dot((-bodyVel), *plane_) * *plane_;
		//Find final velocity 2.0f
		body->setVel(bodyVel + 0.2f * projectionVec);
	}

}

bool PhysicsCollision::BoxSphereCollision(Ref<Actor>& actor_, const Ref<Box>& box_){
	bool status = false;
	Ref<BodyComponent> actorBody = actor_->GetComponent<BodyComponent>();
	Ref<CollisionComponent> actorCollision = actor_->GetComponent<CollisionComponent>();

	if (actorBody && actorCollision) {
		Ref<Sphere> sphere_ = std::dynamic_pointer_cast<Sphere>(actorCollision->getShape());
		float distance = VMath::mag(sphere_->getPos() - box_->centre);
		//Check x sides (0, 3)
		if (PlaneSphereCollision(actorCollision, box_->sides[0]) ||
			PlaneSphereCollision(actorCollision, box_->sides[3])) {
			float expectedDistance = sphere_->r + abs(box_->halfExtents.x);
			if (distance < expectedDistance) {
				std::cout << "BoxSphereColliding x\n";
				return true;
			}
		}
		//Check y sides (1, 4)
		if (PlaneSphereCollision(actorCollision, box_->sides[1]) ||
			PlaneSphereCollision(actorCollision, box_->sides[4])) {
			float expectedDistance = sphere_->r + abs(box_->halfExtents.y);
			if (distance < expectedDistance) {
				std::cout << "BoxSphereColliding y\n";
				return true;
			}
		}
		//Check z sides (2, 5)
		if (PlaneSphereCollision(actorCollision, box_->sides[2]) ||
			PlaneSphereCollision(actorCollision, box_->sides[5])) {
			float expectedDistance = sphere_->r + abs(box_->halfExtents.z);
			if (distance < expectedDistance) {
				std::cout << "BoxSphereColliding z\n";
				return true;
			}
		}
	}

	return false;
}
