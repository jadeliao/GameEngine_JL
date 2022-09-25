#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
using namespace MATH;
template<typename T>
using Ref = std::shared_ptr<T>;

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;

class Scene0 : public Scene {
private:
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	Ref<Actor> mario;
	Ref<Actor> hammer;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};


#endif // SCENE0_H