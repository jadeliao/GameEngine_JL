#ifndef BOX_H
#define BOX_H

#include "Shape.h"
#include <glew.h>
#include <vector>
#include "Vector.h"
#include "QMath.h"
#include "Quaternion.h"


using namespace MATH;

namespace GEOMETRY {
	struct Box : public Shape {
		Vec3 centre;
		Vec3 halfExtents;
		Quaternion orientation;
		Box();
		Box(Vec3 centre_, Vec3 halfExtents_, Quaternion orientation_);
		void set(Vec3 centre_, Vec3 halfExtents_, Quaternion orientation_);
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;

	};
}



#endif

