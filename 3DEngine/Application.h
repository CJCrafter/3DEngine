#pragma once
#include "Timer.h"
#include "Window.h"

class Application
{
public:
	Application();

	int Start();
	void Render();
	void Update();

	Window window;
	Timer timer;
};