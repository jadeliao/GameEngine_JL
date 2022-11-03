#include "CollisionComponent.h"

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

void CollisionComponent::Update(const float deltaTime_)
{
}

void CollisionComponent::Render() const
{
	shape->debugDraw();
}
