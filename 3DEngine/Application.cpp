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
}

void Application::Update()
{
	std::ostringstream title;
	title << timer.Elapsed() << "s";
	window.SetTitle(title.str());
}