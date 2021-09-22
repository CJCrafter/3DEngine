#pragma once

#include "Timer.h"
#include "Window.h"
#include "DrawableBase.h"

class Application
{
public:
	Application();

	int Start();
	void Render();
	void Update();
	
	Window window;
	Timer timer;
private:
	std::vector<std::unique_ptr<Drawable>> shapes;
};
