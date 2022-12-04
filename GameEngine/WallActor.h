#ifndef WALLACTOR_H
#define WALLACTOR_H

#include "Actor.h"
#include "QMath.h"
#include "Quaternion.h"
#include "Box.h"

using namespace GEOMETRY;

class WallActor : public Actor{
private:
	Ref<Box> wallShape;
	bool visibility;
	

public:
	WallActor(Ref<Component> parent_);
	WallActor(const WallActor& actor_);
	~WallActor();

	bool OnCreate() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	void Render() const;
	void HandleEvents(const SDL_Event& sdlEvent) override;
	void setVisible(bool visible_) { visibility = visible_; }
	bool getVisible() { return visibility; }
};

#endif

