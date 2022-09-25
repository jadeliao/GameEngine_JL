#ifndef SCENE1_H
#define SCENE1_H
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

class Scene1 : public Scene {
private:
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	Ref<Actor> gameboard;
	Ref<Actor> checker_red;
	Ref<Actor> checker_black;

public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};


#endif // SCENE1_H