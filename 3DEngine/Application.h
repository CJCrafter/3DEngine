#pragma once

#include "Timer.h"
#include "Window.h"
#include "DrawableBase.h"
#include "ImguiManager.h"

class Application
{
public:
	Application();

	int Start();
	void Render();
	void Update(float delta);

	ImguiManager imgui;
	Window window;
	Timer timer;
private:
	std::vector<std::unique_ptr<Drawable>> shapes;
};
