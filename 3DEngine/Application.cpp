#include "Application.h"

#include "Vec3.h"

Application::Application()
	:
	window(800, 800, "Testing testing 123"),
	timer(0.05f)
{}

int Application::Start()
{
	if (false)
	{
		std::ostringstream stream;
		Vec3f vector1(3.0f, 4.0f, 5.0f);
		Vec3f vector2(vector1);

		stream << "Vector 1: " << vector1 << std::endl
			<< "Vector 1: " << (vector1 *= 2.0f) << std::endl
			<< "Vector 2: " << vector2 << std::endl
			<< "Add     : " << (vector1 + vector2) << std::endl
			<< "Rotate  : " << (Vec3(5, 0, 0).RotateY(3.1415 / 2));

		MessageBox(nullptr, stream.str().c_str(), "Test", MB_OK | MB_ICONEXCLAMATION);
	}


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
	window.GFX().DrawTriangle(timer.Elapsed(), window.mouse.GetX() / (window.GetWidth() / 2.0f) - 1.0f, -(window.mouse.GetY() / (window.GetHeight() / 2.0f) - 1.0f));
	window.GFX().DrawTriangle(timer.Elapsed(), 0, 0);
	window.GFX().Present();
}

void Application::Update()
{
	std::ostringstream title;
	title << timer.Elapsed() << "s";
	std::string str = title.str();
	window.SetTitle(str);
}