#include "LightActor.h"
#include "UBO_Padding.h"

LightActor::LightActor(Ref<Component> parent_, LightStyle lightStyle_, Vec3 location_, Vec4  colour_,
	float intensity_, Vec3 fallOff_ ): Actor(parent_){
	lightStyle = lightStyle_;
	position = location_;
	colour = colour_;
	intensity = intensity_;
	fallOff = fallOff_;
}
LightActor::~LightActor() {
	OnDestroy();
}

bool LightActor::OnCreate(){
	if (isCreated) 	return isCreated; //or return true

	//Allocate space for a VEC3 (position) and VEC4(colour)
	size_t buffer_size = UBO_PADDING::VEC3 + UBO_PADDING::VEC4;
	glGenBuffers(1, &uboLightDataID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightDataID);
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	//Start offset from the start
	size_t offset = 0;
	//Put position(Vec3) into subbuffer
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec3), position);
	//Start the offset at the end of position(Vec3)
	offset = UBO_PADDING::VEC3;
	//Put colour(Vec4) into sub uffer
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), colour);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, uboLightDataID);

	isCreated = true;
	return isCreated;
}

void LightActor::OnDestroy() {
	glDeleteBuffers(1, &uboLightDataID);
	isCreated = false;
}

void LightActor::Render() const{
	glBindBuffer(GL_UNIFORM_BUFFER, GetLightDataID());
}
