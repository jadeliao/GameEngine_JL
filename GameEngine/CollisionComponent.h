// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder

#ifndef COLLISIONCOMPONENT_H
#define COLLISIONOMPONENT_H

#include "Component.h"
#include "Shape.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Capsule.h"
#include "Box.h"

enum class ShapeType {
	sphere,
	cylinder,
	capsule,
	box
};

class CollisionComponent: public Component{
	CollisionComponent(const CollisionComponent&) = delete;
	CollisionComponent(CollisionComponent&&) = delete;
	CollisionComponent& operator = (const CollisionComponent&) = delete;
	CollisionComponent& operator = (CollisionComponent&&) = delete;

public:
	CollisionComponent(Ref<Component> parnet_, std::string shpaeType, Ref<GEOMETRY::Shape>);
	//CollisionComponent(Ref<Component> parnet_, Ref<GEOMETRY::Cylinder>);
	//CollisionComponent(Ref<Component> parnet_, Ref<GEOMETRY::Capsule>);
	//CollisionComponent(Ref<Component> parnet_, Ref<GEOMETRY::Box>);

	Ref<GEOMETRY::Shape> shape;
	ShapeType shapeType;
	virtual ~CollisionComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
};

#endif
