#ifndef CYLINDER_H
#define CYLINDER_H
#include <glew.h>
#include <vector>
#include "Vector.h"
#include "Shape.h"

using namespace MATH;

namespace GEOMETRY {
	struct Cylinder : public Shape{
		float r = 1.0f;
		Vec3 capCentrePosA;
		Vec3 capCentrePosB;
		Cylinder();
		Cylinder(float r_, Vec3 capCentrePosA_, Vec3 capCentrePosB_);
		~Cylinder();
		void set(float r_, Vec3 capCentrePosA_, Vec3 capCentrePosB_);
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
		void setPos(Vec3 pos_) override {}

	};
}
#endif

