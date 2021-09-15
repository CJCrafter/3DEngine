#include "Application.h"

#include <random>

#include "Vec3.h"
#include "Cube.h"

Application::Application()
	:
	window(800, 800, "Testing testing 123"),
	timer(0.05f)
{
	std::mt19937 rand(std::random_device{}());
	std::uniform_real_distribution<float> a(0.0f, 3.1415 * 2.0f);
	std::uniform_real_distribution<float> b(0.0f, 3.1415 * 2.0f);
	std::uniform_real_distribution<float> c(0.0f, 3.1415 * 0.3f);
	std::uniform_real_distribution<float> d(6.0f, 20.0f);

	for (int i = 0; i < 80; i++)
	{
		cubes.push_back(std::make_unique<Cube>(
			window.GFX(), rand, a, b, c, d
		));
	}
	window.GFX().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 40.0f));
}

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
	window.GFX().Clear(0.07f, 0.0f, 0.12f);
	const float delta = timer.Elapsed();
	timer.Mark();
	for (auto& cube : cubes)
	{
		cube->Update(delta);
		cube->Draw(window.GFX());
	}
	window.GFX().Present();
}

void Application::Update()
{
	std::ostringstream title;
	title << timer.Elapsed() << "s";
	std::string str = title.str();
	window.SetTitle(str);
}