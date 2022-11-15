#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Window.h"

class UserInterface{

protected:
	class Window* window;

public:
	UserInterface();
	~UserInterface();
	void OnDestroy();
	bool OnCreate(Window* window_);
	virtual void Update(const float deltaTime_) = 0;
	void Render() const;
	void HandleEvents();

};

#endif

