#include "Capsule.h"
#include "MMath.h"
#include "Matrix.h"

using namespace GEOMETRY;

GEOMETRY::Capsule::Capsule(): sphereCentrePosA(Vec3(0.0f, 0.0f, 0.0f)), sphereCentrePosB(Vec3(0.0f, 1.0f, 0.0f)) {
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);
}

GEOMETRY::Capsule::Capsule(float r_, Vec3 sphereCentrePosA_, Vec3 sphereCentrePosB_) {
	set(r_, sphereCentrePosA_, sphereCentrePosB_);
}

GEOMETRY::Capsule::~Capsule() {

}

void GEOMETRY::Capsule::set(float r_, Vec3 sphereCentrePosA_, Vec3 sphereCentrePosB_) {
	r = r_;
	sphereCentrePosA = sphereCentrePosA_;
	sphereCentrePosB = sphereCentrePosB_;
	generateVerticesAndNormals();
	StoreMeshData(GL_POINTS);
}

void GEOMETRY::Capsule::generateVerticesAndNormals(){
	const float deltaTheta = 1.0f;
	const float deltaPhi = 360.0f / 15.0f;
	Vec3 lengthVector = sphereCentrePosB - sphereCentrePosA;
	const float capsuleLength = VMath::mag(lengthVector);

	//Check magnitude
	if (capsuleLength > VERY_SMALL) {
		const Vec3 axis = VMath::normalize(lengthVector);
		//nudgeAxis to avoid parallel cross product
		const Vec3 nudgeAxis = axis + Vec3(0.01f, 0.01f, 0.01f);
		const Vec3 radialVector = r * VMath::normalize(VMath::cross(axis, nudgeAxis));

		//Draw top half sphere
		for (float thetaDeg = 0.0f; thetaDeg <= 90.0f; thetaDeg += deltaTheta) {
			Vec3 sphere1(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
			//Move the position of the sphere to the top of the cylinder
			sphere1 = sphere1 + sphereCentrePosA;
			for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
				MATH::Matrix4 rotationMatrix;
				rotationMatrix = MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
				sphere1 = rotationMatrix * sphere1;
				vertices.push_back(sphere1);
				normals.push_back(sphere1);
			}
		}
		//Draw bot half sphere
		for (float thetaDeg = 180.0f; thetaDeg <= 270.0f; thetaDeg += deltaTheta) {
			Vec3 sphere2(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
			//Move the position of the sphere to the botton of the cylinder
			sphere2 = sphere2 + sphereCentrePosB;
			for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
				MATH::Matrix4 rotationMatrix;
				rotationMatrix = MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
				sphere2 = rotationMatrix * sphere2;
				vertices.push_back(sphere2);
				normals.push_back(sphere2);
			}
		}

		//Draw Cylinder
		//Loop a circle
		for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
		{
			//Loop vertically and draw the cylinder
			for (float stepsAlongAxis = 0.0f; stepsAlongAxis <= (capsuleLength + 0.1f); stepsAlongAxis += 0.1f)
			{
				Vec3 circle = sphereCentrePosA + radialVector + axis * stepsAlongAxis;
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

RayIntersectionInfo GEOMETRY::Capsule::rayIntersectionInfo(const Ray& ray) const
{
	return RayIntersectionInfo();
}

