#ifndef BOX_H
#define BOX_H

#include "Shape.h"
#include <glew.h>
#include <vector>
#include "Vector.h"
#include "QMath.h"
#include "Quaternion.h"
#include "Plane.h"

template<typename T>
using Ref = std::shared_ptr<T>;

using namespace MATH;

namespace GEOMETRY {
	struct Box : public Shape {
		Ref<GEOMETRY::Plane> sides[6];
		Vec3 centre;
		Vec3 halfExtents;
		Quaternion orientation;
		Box();
		Box(Vec3 centre_, Vec3 halfExtents_, Quaternion orientation_);
		void set(Vec3 centre_, Vec3 halfExtents_, Quaternion orientation_);
		void generateVerticesAndNormals() override;
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
		void setPos(Vec3 pos_) override { centre = pos_; }
	};
}



#endif

