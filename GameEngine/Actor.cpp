#include "Actor.h"
#include "Debug.h"
#include <SDL.h>
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "BodyComponent.h"
#include "AIComponent.h"

Actor::Actor(Ref<Component> parent_):Component(parent_) {}

//Deep Copy constructor
Actor::Actor(const Actor& actor_){
	std::cout << "Copying actor\n";
	std::vector<Ref<Component>> new_List = std::vector<Ref<Component>>();
	new_List = actor_.GetComponentsList();
	components = new_List;
	parent = actor_.GetParent();
}

bool Actor::OnCreate() {
	if (isCreated) return isCreated;

	for (auto component : components) {
		if (component->OnCreate() == false) {
			cout << typeid(this).name() << "\n";
			Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
			isCreated = false;
		}
	}

	isCreated = true;
	return isCreated;
}

Actor::~Actor() {
	OnDestroy();
}

void Actor::OnDestroy() {
	RemoveAllComponents();
	Debug::Info("Deleting assets for Actor: ", __FILE__, __LINE__);
	isCreated = false;
}

void Actor::Update(const float deltaTime) {
	for (auto component : components) {
		component->Update(deltaTime);
	}

	GetModelMatrix();
}

void Actor::HandleEvents(const SDL_Event& sdlEvent){
}

void Actor::Render()const {
	Ref<MaterialComponent> texture = GetComponent<MaterialComponent>();
	Ref<ShaderComponent> shader = GetComponent<ShaderComponent>();
	Ref<MeshComponent> mesh = GetComponent<MeshComponent>();
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);
}

void Actor::RemoveAllComponents() {
	for (auto component : components) {
		component->OnDestroy();
	}
	components.clear();
}

void Actor::ListComponents() const {
	std::cout << typeid(*this).name() << " contains the following components:\n";
	for (auto component : components) {
		std::cout << typeid(*component).name() << std::endl;
	}
	std::cout << '\n';
}

Matrix4 Actor::GetModelMatrix() {

	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
	if (transformComponent) { /// There is no such component, use default view
		modelMatrix = transformComponent->GetTransformMatrix();
	}
	else {
		modelMatrix.loadIdentity();
	}

	if (parent) {
		modelMatrix = std::dynamic_pointer_cast<Actor>(parent)->GetModelMatrix() * modelMatrix;
	}
	return modelMatrix;
}