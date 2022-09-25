#include <glew.h>
#include <SDL.h>
#include "CameraActor.h"
#include "MMath.h"
#include "QMath.h"
#include "TransformComponent.h"
#include "Debug.h"
#include "UBO_Padding.h"
using namespace MATH;

CameraActor::CameraActor(Ref<Component> parent_): Actor(parent_) {
	
}

CameraActor::~CameraActor() {
	OnDestroy();
}

bool CameraActor::OnCreate() {
	if (isCreated) 	return isCreated; //or return true

	Debug::Info("Creating values for CameraActor: ", __FILE__, __LINE__);
	size_t buffer_size = 2 * UBO_PADDING::MAT4;
	glGenBuffers(1, &uboMatriciesID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatriciesID);
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboMatriciesID);

	UpdateProjectionMatrix(45.0f, (16.0f / 9.0f), 0.5f, 100.0f); /// default projection
	UpdateViewMatrix();

	isCreated = true;
	return isCreated;
}

void CameraActor::OnDestroy(){
	glDeleteBuffers(1, &uboMatriciesID);
	isCreated = false;
}

void CameraActor::Render() const {
	glBindBuffer(GL_UNIFORM_BUFFER, GetMatriciesID());

}

void CameraActor::HandleEvents(const SDL_Event & sdlEvent){
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			std::cout << "LEFT EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(-1.0f, 0.0f, 0.0f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			std::cout << "RIGHT EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(1.0f, 0.0f, 0.0f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			std::cout << "UP EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, 1.0f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			std::cout << "DOWN EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(
				transform_->GetPosition() + Vec3(0.0f, 0.0f, -1.0f), transform_->GetQuaternion());
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			std::cout << "Q EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			std::cout << "E EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			std::cout << "W EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(-2.0f, Vec3(1.0f, 0.0f, 0.0f)));
			UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			std::cout << "S EVENT\n";
			Ref<TransformComponent> transform_ = GetComponent<TransformComponent>();
			transform_->SetTransform(transform_->GetPosition(), transform_->GetQuaternion() *
				QMath::angleAxisRotation(2.0f, Vec3(1.0f, 0.0f, 0.0f)));
			UpdateViewMatrix();
		}
		break;
	default:
		break;
	}
}

void CameraActor::UpdateViewMatrix() {
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
	if (transformComponent.get() == nullptr) { /// There is no such component, use default view
		viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		Quaternion orientation = transformComponent->GetQuaternion();
		Vec3 position = transformComponent->GetPosition();
		viewMatrix = MMath::translate(position) * QMath::toMatrix4(orientation);
	}
	size_t offset = sizeof(Matrix4);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatriciesID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), viewMatrix);

}

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {
	size_t offset = 0;
	projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatriciesID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), projectionMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}