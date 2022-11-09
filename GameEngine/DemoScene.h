#ifndef DEMOSCENE_H
#define DEMOSCENE_H


#include "SceneActor.h"
#include <vector>
#include <iostream>

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;

class DemoScene : public SceneActor{
public:

	explicit DemoScene();
	virtual ~DemoScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime);
	virtual void HandleEvents(const SDL_Event& sdlEvent);

};

#endif
