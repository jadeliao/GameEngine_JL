#ifndef DEMOSCENE2_H
#define DEMOSCENE2_H


#include "SceneActor.h"
#include <vector>
#include <iostream>

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class CameraActor;
class LightActor;
class Actor;

class DemoScene2 : public SceneActor{

private:
	float actInterval = 0.0f;
	std::vector<int> pathList;
public:

	explicit DemoScene2();
	virtual ~DemoScene2();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime);
	virtual void HandleEvents(const SDL_Event& sdlEvent);

};

#endif
