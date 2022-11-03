#include "Sphere.h"
#include "MMath.h"
#include "Matrix.h"
#include "QuadraticSolution.h"

using namespace MATH;
using namespace GEOMETRY;

void Sphere::generateVerticesAndNormals() {
	const float deltaTheta = 1.0f;
	const float deltaPhi = 360.0f / 15.0f;
	for (float thetaDeg = 0.0f; thetaDeg <= 180.0f; thetaDeg += deltaTheta)
	{
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
		for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;
			vertices.push_back(circle);
			normals.push_back(circle - Vec3(x, y, z));
		}
	}
}

RayIntersectionInfo Sphere::rayIntersectionInfo(const Ray& ray) const {
	RayIntersectionInfo rayInfo;
	Vec3 sphereToRayStart = ray.start - Vec3(x, y, z);
	
	//Find the abc expression used for quadratic equation
	const float a = VMath::dot(ray.dir, ray.dir); // V^2
	const float b = 2.0f * VMath::dot(sphereToRayStart, ray.dir); // 2(s-c)v
	const float c = VMath::dot(sphereToRayStart, sphereToRayStart) - r * r; // (s-c)^2 - r^2
	//Solve quadratic equation
	QuadraticSolution soln = solveQuadratic(a, b, c);
	//Check intersection conditions
	if (soln.numSolutions == NumSolutions::zeroRoots) { // no collision
		rayInfo.isIntersected = false;
	}
	else if (soln.numSolutions == NumSolutions::oneRoot) {
		// Only intersects when t is positive
		if (soln.firstRoot > 0) { 
			rayInfo.isIntersected = true;
			rayInfo.intersectionPoint = ray.currentPosition(soln.firstRoot);
		}
		else {
			rayInfo.isIntersected = false;
		}
	}
	else if (soln.numSolutions == NumSolutions::twoRoot) {
		float t1 = soln.firstRoot;
		float t2 = soln.secondRoot;
		//if both ts are positive, t1 is the closest
		if (t1 >= 0 && t2 >= 0) { 
			rayInfo.isIntersected = true;
			rayInfo.intersectionPoint = ray.currentPosition(t1);
		}
		//if both ts are negative, no collision occurs
		else if (t1 < 0 && t2 < 0) { 
			rayInfo.isIntersected = false;
		}
		else {
			//if t1 is positive, t1 is where collision occurs
			if (t1 >= 0 && t2 < 0) { 
				rayInfo.isIntersected = true;
				rayInfo.intersectionPoint = ray.currentPosition(t1);
			}
			//if t2 is positive, t2 is where collision occurs
			else if (t1 < 0 && t2 >= 0) { 
				rayInfo.isIntersected = true;
				rayInfo.intersectionPoint = ray.currentPosition(t2);
			}
		}
	}
	return rayInfo;
}