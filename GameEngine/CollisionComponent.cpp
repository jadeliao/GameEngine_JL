#include "CollisionComponent.h"
#include "Actor.h"
#include "TransformComponent.h"

CollisionComponent::CollisionComponent(Ref<Component> parent_, std::string shapeType_, Ref<GEOMETRY::Shape> shape_) :Component(parent_), shape(shape_) {
	if (shapeType_ == "sphere") {
		shapeType = ShapeType::sphere;
	}
	else if (shapeType_ == "cylinder") {
		shapeType = ShapeType::cylinder;
	}
	else if (shapeType_ == "capsule"){
		shapeType = ShapeType::capsule;
	}
	else if (shapeType_ == "box") {
		shapeType = ShapeType::box;
	}
	else if (shapeType_ == "plane") {
		shapeType = ShapeType::plane;
	}
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::OnCreate()
{
	return true;
}

void CollisionComponent::OnDestroy()
{
}

void CollisionComponent::Update(const float deltaTime_){
	//Set the new values to transform component
	Ref<Actor> parentActor = std::dynamic_pointer_cast<Actor>(GetParent());
	if (parentActor) {
		Ref<TransformComponent> transform = parentActor->GetComponent<TransformComponent>();
		Vec3 newPos = transform->GetPosition();
		shape->setPos(newPos);
	}
}

void CollisionComponent::Render() const
{
	shape->debugDraw();
}
