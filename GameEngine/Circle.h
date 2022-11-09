#ifndef CIRCLE_H
#define CIRCLE_H
#include "Vector.h"
#include <iostream>
namespace  MATH {
	struct Circle : public Vec3 {
		float r;

		/// Just a little utility to populate a Shere
		inline void set(float x_, float y_, float z_, float r_) {
			x = x_; y = y_; z = z_, r = r_;
		}

		Circle() {
			x = 0.0f; y = 0.0f; z = 0.0f; r = 0.0f; 
		}

		Circle(float x, float y, float z, float r) {
			set(x, y, z, r);
		}

		Circle(const Circle& s) {
			set(s.x, s.y, s.z, s.r);
		}

		/// print the values of the sphere and add a comment if you wish
		void print(const char* comment = nullptr) {
			if (comment) printf("%s\n", comment);
			printf("%f %f %f %f\n", x, y, z, r);
		}
	};


	

}
#endif