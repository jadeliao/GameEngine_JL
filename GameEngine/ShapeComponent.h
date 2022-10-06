#pragma once
#include "Component.h"
class ShapeComponent : public Component {
private:

public:
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
};

