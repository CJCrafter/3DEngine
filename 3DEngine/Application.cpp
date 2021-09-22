#include "Application.h"

#include <random>

#include "Block.h"
#include "PointList.h"
#include "Sphere.h"
#include "Vec3.h"

Application::Application()
	:
	window(800, 800, "Testing testing 123"),
	timer(0.005f)
{
	std::mt19937 rand(std::random_device{}());
	std::uniform_real_distribution a(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution b(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution c(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution d(6.0f, 20.0f);
	std::uniform_real_distribution e(0.25f, 2.0f);
	std::uniform_real_distribution f(0.0f, 1.0f);

	Sphere sphere(1000);
	for (int i = 0; i < 1; i++)
	{
		shapes.push_back(std::make_unique<PointList>(window.GetGraphics(), sphere));
		auto& temp = shapes.back();
		//temp->position = { a(rand), a(rand), a(rand) };
		//temp->velocity = { c(rand), c(rand), c(rand) };
		temp->rotation = { b(rand), b(rand), b(rand) };
		temp->rotation /= 4.0f;
	}
	window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 40.0f));
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
	Graphics& graphics = window.GetGraphics();
	graphics.Clear(0.07f, 0.0f, 0.12f);
	const float delta = timer.Elapsed();
	timer.Mark();

	for (auto& shape : shapes)
	{
		shape->Tick(delta);
		shape->Draw(graphics);
	}
	graphics.Present();
}

void Application::Update()
{
	std::ostringstream title;
	title << timer.Elapsed() << "s";
	std::string str = title.str();
	window.SetTitle(str);
}