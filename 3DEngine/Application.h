#pragma once
#include "Window.h"

class Application
{
	Window window;

public:
	Application();

	int Start();
	void Render();
	void Update();
};