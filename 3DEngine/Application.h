#pragma once

#include "Timer.h"
#include "Window.h"
#include "DrawableBase.h"
#include "ImguiManager.h"
#include "Camera.h"

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
	Camera camera;
	bool isPause = false;
	float speed = 1.0;
private:
	std::vector<std::unique_ptr<Drawable>> shapes;
};
