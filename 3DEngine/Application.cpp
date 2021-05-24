#include "Application.h"

Application::Application()
	:
	window(800, 800, L"Testing testing 123")
{}

int Application::Start()
{
	// Message pump. We want to handle messages until an error occurs, or the application is quit
	MSG msg;
	BOOL result;
	while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		Update();
		Render();
	}

	if (result == -1)
	{
		throw LAST_EXCEPTION();
	}
	return msg.wParam;
}

void Application::Render()
{
}

void Application::Update()
{
}