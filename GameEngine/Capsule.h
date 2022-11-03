#ifndef CAPSULE_H
#define CAPSULE_H
#include <glew.h>
#include <vector>
#include "Vector.h"
#include "Shape.h"

using namespace MATH;

namespace GEOMETRY {
	struct Capsule : public Shape {
		float r = 1.0f;
		Vec3 sphereCentrePosA;
		Vec3 sphereCentrePosB;
		Capsule();
		Capsule(float r_, Vec3 sphereCentrePosA_, Vec3 sphereCentrePosB_);
		~Capsule();
		void set(float r_, Vec3 sphereCentrePosA_, Vec3 sphereCentrePosB_);
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;

	};
}


#endif

