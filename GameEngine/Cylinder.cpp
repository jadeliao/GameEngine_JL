#include "Cylinder.h"
#include "MMath.h"
#include "Matrix.h"

using namespace MATH;
using namespace GEOMETRY;


GEOMETRY::Cylinder::Cylinder(): capCentrePosA(Vec3(0.0f, 0.0f, 0.0f)), capCentrePosB(Vec3(0.0f, 1.0f, 0.0f)) {
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);
}

GEOMETRY::Cylinder::Cylinder(float r_, Vec3 capCentrePosA_, Vec3 capCentrePosB_){
	set(r_, capCentrePosA_, capCentrePosB_);
}

GEOMETRY::Cylinder::~Cylinder(){}

void GEOMETRY::Cylinder::set(float r_, Vec3 capCentrePosA_, Vec3 capCentrePosB_){
	r = r_;
	capCentrePosA = capCentrePosA_;
	capCentrePosB = capCentrePosB_;
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);
}

void GEOMETRY::Cylinder::generateVerticesAndNormals(){

	const float deltaTheta = 1.0f;
	Vec3 lengthVector = capCentrePosB - capCentrePosA;
	const float cylinderLength = VMath::mag(lengthVector);

	//Check magnitude
	if (cylinderLength > VERY_SMALL) {
		const Vec3 axis = VMath::normalize(lengthVector);
		//nudgeAxis to avoid parallel cross product
		const Vec3 nudgeAxis = axis + Vec3(0.01f, 0.01f, 0.01f);
		const Vec3 radialVector = r * VMath::normalize(VMath::cross(axis, nudgeAxis));
		//Loop a circle
		for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
		{
			//Loop vertically
			for (float stepsAlongAxis = 0.0f; stepsAlongAxis < cylinderLength; stepsAlongAxis += 0.1f)
			{
				Vec3 circle = capCentrePosA + radialVector + axis * stepsAlongAxis;
				Matrix3 rotationMatrix = MATH::MMath::rotate(thetaDeg, axis);
				circle = rotationMatrix * circle;
				vertices.push_back(circle);
				normals.push_back(circle);
			}
		}
	}
	else {
		std::cerr << "Magnitude too small\n";
	}
}

RayIntersectionInfo GEOMETRY::Cylinder::rayIntersectionInfo(const Ray& ray) const
{
	return RayIntersectionInfo();
}
