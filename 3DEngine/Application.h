#pragma once
#include "Timer.h"
#include "Window.h"
#include "Cube.h"

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
	std::vector<std::unique_ptr<class Cube>> cubes;
};
