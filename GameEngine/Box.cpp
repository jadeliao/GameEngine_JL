#include "Box.h"

using namespace MATH;
using namespace GEOMETRY;

GEOMETRY::Box::Box(){
	generateVerticesAndNormals();
	StoreMeshData(GL_TRIANGLES);
}

GEOMETRY::Box::Box(Vec3 centre_, Vec3 halfExtents_, Quaternion orientation_){
	set(centre_, halfExtents_, orientation_);
}

void GEOMETRY::Box::set(Vec3 centre_, Vec3 halfExtents_, Quaternion orientation_){
	centre = centre_;
	halfExtents = halfExtents_;
	orientation = orientation_;
	//Set six sides
	sides[0] = std::make_shared<Plane>(1.0f, 0.0f, 0.0f, centre_.x + halfExtents_.x);
	sides[1] = std::make_shared<Plane>(0.0f, 1.0f, 0.0f, centre_.y + halfExtents_.y);
	sides[2] = std::make_shared<Plane>(0.0f, 0.0f, 1.0f, centre_.z + halfExtents_.z);
	sides[3] = std::make_shared<Plane>(-1.0f, 0.0f, 0.0f, centre_.x + -halfExtents_.x);
	sides[4] = std::make_shared<Plane>(0.0f, -1.0f, 0.0f, centre_.y + -halfExtents_.y);
	sides[5] = std::make_shared<Plane>(0.0f, 0.0f, -1.0f, centre_.z + -halfExtents_.z);

	generateVerticesAndNormals();
	StoreMeshData(GL_TRIANGLES);
}

void GEOMETRY::Box::generateVerticesAndNormals(){

	std::vector<Vec3> uniqueVertices;

	//Find all 8 vertices used for rendering
	const int top_right_front = 0;
	uniqueVertices.push_back(orientation * (centre + Vec3(halfExtents.x, halfExtents.y, halfExtents.z)));
	const int top_right_back = 1;
	uniqueVertices.push_back(orientation * (centre + Vec3(halfExtents.x, -halfExtents.y, halfExtents.z)));
	const int top_left_front = 2;
	uniqueVertices.push_back(orientation * (centre + Vec3(-halfExtents.x, halfExtents.y, halfExtents.z)));
	const int top_left_back = 3;
	uniqueVertices.push_back(orientation * (centre + Vec3(-halfExtents.x, -halfExtents.y, halfExtents.z)));
	const int bot_right_front = 4;
	uniqueVertices.push_back(orientation * (centre + Vec3(halfExtents.x, halfExtents.y, -halfExtents.z)));
	const int bot_right_back = 5;
	uniqueVertices.push_back(orientation * (centre + Vec3(halfExtents.x, -halfExtents.y, -halfExtents.z)));
	const int bot_left_front = 6;
	uniqueVertices.push_back(orientation * (centre + Vec3(-halfExtents.x, halfExtents.y, -halfExtents.z)));
	const int bot_left_back = 7;
	uniqueVertices.push_back(orientation * (centre + Vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z)));

	//Top side
	vertices.push_back(uniqueVertices[top_right_front]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[top_left_front]);
	vertices.push_back(uniqueVertices[top_left_front]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[top_left_back]);
	//Normal vertices
	for (int i = 0; i < 6; i++) {
		normals.push_back(Vec3(0.0f, 1.0f, 0.0));
	}
	//Bottom Side
	vertices.push_back(uniqueVertices[bot_right_front]);
	vertices.push_back(uniqueVertices[bot_right_back]);
	vertices.push_back(uniqueVertices[bot_left_front]);
	vertices.push_back(uniqueVertices[bot_left_front]);
	vertices.push_back(uniqueVertices[bot_right_back]);
	vertices.push_back(uniqueVertices[bot_left_back]);
	//Normal vertices
	for (int i = 0; i < 6; i++) {
		normals.push_back(Vec3(0.0f, -1.0f, 0.0));
	}
	//Right Side
	vertices.push_back(uniqueVertices[top_right_front]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[bot_right_front]);
	vertices.push_back(uniqueVertices[bot_right_front]);
	vertices.push_back(uniqueVertices[bot_right_back]);
	vertices.push_back(uniqueVertices[top_right_back]);
	//Normal vertices
	for (int i = 0; i < 6; i++) {
		normals.push_back(Vec3(1.0f, 0.0f, 0.0));
	}
	//Left Side
	vertices.push_back(uniqueVertices[top_left_front]);
	vertices.push_back(uniqueVertices[top_left_back]);
	vertices.push_back(uniqueVertices[bot_left_front]);
	vertices.push_back(uniqueVertices[bot_left_front]);
	vertices.push_back(uniqueVertices[bot_left_back]);
	vertices.push_back(uniqueVertices[top_left_back]);
	//Normal vertices
	for (int i = 0; i < 6; i++) {
		normals.push_back(Vec3(0.0f, -1.0f, 0.0));
	}
	//Front Side
	vertices.push_back(uniqueVertices[top_left_front]);
	vertices.push_back(uniqueVertices[top_right_front]);
	vertices.push_back(uniqueVertices[bot_left_front]);
	vertices.push_back(uniqueVertices[top_right_front]);
	vertices.push_back(uniqueVertices[bot_left_front]);
	vertices.push_back(uniqueVertices[bot_right_front]);
	//Normal vertices
	for (int i = 0; i < 6; i++) {
		normals.push_back(Vec3(0.0f, 0.0f, 1.0));
	}
	//Back Side
	vertices.push_back(uniqueVertices[top_left_back]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[bot_left_back]);
	vertices.push_back(uniqueVertices[top_right_back]);
	vertices.push_back(uniqueVertices[bot_left_back]);
	vertices.push_back(uniqueVertices[bot_right_back]);
	//Normal vertices
	for (int i = 0; i < 6; i++) {
		normals.push_back(Vec3(0.0f, 0.0f, 0.0));
	}

}

RayIntersectionInfo GEOMETRY::Box::rayIntersectionInfo(const Ray& ray) const {

	const int slabNum = 3;

	// Imagine a box is just made up of three infinite slabs
	Slab slabX;
	slabX.normal = Vec3(1.0f, 0.0f, 0.0f);
	slabX.distNear = -halfExtents.x;
	slabX.distFar = halfExtents.x;

	Slab slabY;
	slabY.normal = Vec3(0.0f, 1.0f, 0.0f);
	slabX.distNear = -halfExtents.y;
	slabX.distFar = halfExtents.y;

	Slab slabZ;
	slabY.normal = Vec3(0.0f, 0.0f, 1.0f);
	slabX.distNear = -halfExtents.z;
	slabX.distFar = halfExtents.z;

	Slab slabs[slabNum] = { Slab{Vec3(1.0f, 0.0f, 0.0f), -halfExtents.x, halfExtents.x},
						Slab{Vec3(0.0f, 1.0f, 0.0f), -halfExtents.y, halfExtents.y},
						Slab{Vec3(0.0f, 0.0f, 1.0f), -halfExtents.z, halfExtents.z} };

	float tMin = 0.0f;
	float tMax = FLT_MAX;

	RayIntersectionInfo rayInfo;
	for (int i = 0; i < slabNum; i++) {
		float t1 = (slabs[i].distNear - ray.start[i]) / ray.dir[i];
		float t2 = (slabs[i].distFar - ray.start[i]) / ray.dir[i];

		if (t1 > t2) { std::swap(t1, t2); }
		tMin = std::max(tMin, t1);
		tMax = std::min(tMax, t2);

		if (tMin > tMax) return rayInfo;
	}
	rayInfo.isIntersected = true;
	rayInfo.t = tMin;
	rayInfo.intersectionPoint = ray.currentPosition(tMin);
	return rayInfo;

	return RayIntersectionInfo();
}