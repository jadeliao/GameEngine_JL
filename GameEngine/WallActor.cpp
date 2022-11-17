#include "WallActor.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"

WallActor::WallActor(Ref<Component> parent_):Actor(parent_), visibility(false) {

}

//Deep Copy constructor
WallActor::WallActor(const WallActor& actor_): Actor(actor_), visibility(false) {

}


WallActor::~WallActor() {
	OnDestroy();
}

bool WallActor::OnCreate(){
	return Actor::OnCreate();
}

void WallActor::Update(float deltaTime){
	Actor::Update(deltaTime);
	//GetComponent<TransformComponent>()->GetTransformMatrix().print();
}

void WallActor::OnDestroy(){
	isCreated = false;
}

void WallActor::Render() const{
	if (visibility) {

		Actor::Render();
	}

}

void WallActor::HandleEvents(const SDL_Event& sdlEvent){
}
