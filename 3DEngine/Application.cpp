#include "Application.h"

Application::Application()
	:
	window(800, 800, L"Testing testing 123"),
	timer(0.05f)
{}

int Application::Start()
{
	while (true)
	{
		if (const auto code = Window::ProcessMessages())
		{
			return *code;
		}
		Update();
		Render();
	}
}

void Application::Render()
{
	const float c = sin(timer.Elapsed()) / 2.0 + 0.5;
	window.GFX().Clear(c, c, 1.0f);
	window.GFX().DrawTriangle();
	window.GFX().Present();
}

void Application::Update()
{
	std::ostringstream title;
	title << timer.Elapsed() << "s";
	std::string str = title.str();
	window.SetTitle(str);
}