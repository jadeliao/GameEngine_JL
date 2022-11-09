#ifndef RAY_H
#define RAY_H

#include "Vector.h"
#include "VMath.h"

using namespace MATH;

namespace GEOMETRY {
	struct RayIntersectionInfo {
		bool isIntersected = false;
		Vec3 intersectionPoint;
		float t = 0.0f; // determine going forewards or backwards

		void print() {
			if (isIntersected) {
				std::cout << "Ray intersection with t value " << t << " at position ";
				intersectionPoint.print();
			}
			else {
				std::cout << "No ray intersection";
			}
		}
	};

	struct Ray {
		Vec3 dir;
		Vec3 start;
		Ray() {
			dir.set(0.0f, 0.0f, -1.0f);
			start.set(0.0f, 0.0f, 0.0f);
		}
		Ray(const Vec3& start_, const Vec3& dir_) {
			set(start_, dir_);
		}
		void set(const Vec3& start_, const Vec3& dir_) {
			start = start_;
			dir = dir_;
		}

		Vec3 currentPosition(float t) const {
			return start + t * dir;
		}

	};
}


#endif
