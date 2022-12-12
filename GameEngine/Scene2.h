#ifndef SCENE2_H0
#define SCENE2_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Component.h"
#include "SceneActor.h"
#include <vector>

using namespace MATH;

#define CHECKER_MAX_NUM 16

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;

class Scene2 : public SceneActor{
private:
	const char* playerName;

public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime);
	virtual void HandleEvents(const SDL_Event& sdlEvent);

};


#endif // SCENE2_H