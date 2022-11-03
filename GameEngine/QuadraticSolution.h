#ifndef QUADRATICSOLUTION_H
#define QUADRATICSOLUTION_H

#include <cmath>
#include <iostream>

#define VERY_SMALL 1.0e-7f

enum class NumSolutions {
	zeroRoots = 0,
	oneRoot,
	twoRoot
};

namespace GEOMETRY {
	struct QuadraticSolution {
		NumSolutions numSolutions{ NumSolutions::zeroRoots };
		float firstRoot = 0.0f;
		float secondRoot = 0.0f;
	};

	QuadraticSolution solveQuadratic(float a, float b, float c) {

		GEOMETRY::QuadraticSolution soln;
		float denominator = 2.0f * a;
		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0.0f) {
			return soln;
		}
		else if (fabs(discriminant) < FLT_MIN) {
			soln.numSolutions = NumSolutions::oneRoot;
			soln.firstRoot = (-b + sqrt(discriminant)) / denominator;
			return soln;
		}
		else {
			soln.numSolutions = NumSolutions::twoRoot;
			float x1 = (-b + sqrt(discriminant)) / denominator;
			float x2 = (-b - sqrt(discriminant)) / denominator;
			//Make first root be the smallest
			soln.firstRoot = std::min(x1, x2);
			soln.secondRoot = std::max(x1, x2);
			return soln;
		}
	}
}



#endif