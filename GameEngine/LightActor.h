#pragma once
#include <glew.h>
#include "Actor.h"
#include "Vector.h"
using namespace MATH;

enum class LightStyle {
	DirectionLight = 1,
	PointLight,
	SkyLight,
	SpotLight
};

class LightActor : public Actor {
private:
	LightStyle lightStyle;
	Vec3 position;
	Vec4 colour;
	float intensity;
	Vec3 fallOff;
	unsigned int uboLightDataID;
	const GLuint bindPoint = 1;
public:
	LightActor(Ref<Component> parent_, LightStyle lightStyle_, Vec3 location_, Vec4  colour_,
		float intensity = 1.0f, Vec3 fallOff = Vec3(0.0f, 0.0f, 0.0f));
	~LightActor();
	bool OnCreate() override;
	void OnDestroy() override;
	void Render() const;
	Vec3 GetPosition() const { return position; }
	GLuint GetLightDataID() const { return uboLightDataID; }



};

