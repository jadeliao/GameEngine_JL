#include "Plane.h"
#include "MMath.h"
#include "Matrix.h"

using namespace MATH;
using namespace GEOMETRY;

RayIntersectionInfo GEOMETRY::Plane::rayIntersectionInfo(const Ray& ray) const
{
	return RayIntersectionInfo();
}

void GEOMETRY::Plane::generateVerticesAndNormals()
{
}
