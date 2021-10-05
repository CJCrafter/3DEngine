#include "Application.h"

#include <random>

#include "Block.h"
#include "Cone.h"
#include "Cube.h"
#include "Cylinder.h"
#include "IcoSphere.h"
#include "MirahCube.h"
#include "PointList.h"
#include "ShapeDrawable.h"
#include "PointSphere.h"
#include "Vec3.h"
#include "GDIPlusManager.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

GDIPlusManager gdi;

Application::Application()
	:
	window(800, 800, "Testing testing 123"),
	timer(0.005f),
	camera(20.0f, 0.0f, 0.0f, {0.0f, 0.0f, 0.0f})
{
	std::mt19937 rand(std::random_device{}());
	std::uniform_real_distribution a(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution b(0.0f, 3.1415f * 0.25f);
	std::uniform_real_distribution c(-3.1415f * 0.3f, 3.1415f * 0.3f);
	std::uniform_real_distribution d(0.2f, 0.4f);
	std::uniform_real_distribution e(0.25f, 2.0f);
	std::uniform_real_distribution f(0.0f, 1.0f);

	for (int i = 0; i < 1; i++)
	{
		shapes.push_back(std::make_unique<MirahCube>(window.GetGraphics()));
		auto& temp = shapes.back();
		//temp->position = { a(rand), a(rand), a(rand) };
		//temp->velocity = { c(rand), c(rand), c(rand) * 2 };
		//temp->rotation = { b(rand), b(rand), b(rand) };
		temp->angle = { PI / 40.0f, PI / 40.0f, 0.0f };
		temp->scale    = Vec3f{ 1.0f, 1.0f, 1.0f } * 10.0f;
	}
	window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 40.0f));
}

int Application::Start()
{
	while (true)
	{
		if (const auto code = Window::ProcessMessages())
		{
			return *code;
		}

		const float delta = timer.Elapsed();
		timer.Mark();

		Keyboard::Event keyPress = window.key.ReadKey();
		if (keyPress.GetCode() == VK_ESCAPE)
		{
			if (keyPress.IsPress())
			{
				isPause = !isPause;
			}
		}

		if (!isPause)
		{
			Update(delta * speed);
		}

		Render();
	}
}

void Application::Render()
{
	Graphics& graphics = window.GetGraphics();
	graphics.Clear(0.07f, 0.0f, 0.12f);
	graphics.SetCamera(camera.GetMatrix());

	for (auto& shape : shapes)
	{
		shape->Draw(graphics);
	}

	if (isPause)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		if (ImGui::Begin("Debug", &show))
		{
			if (ImGui::Button("Resume"))
			{
				isPause = false;
			}
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::SliderFloat("Speed", &speed, 0.25f, 10.0f);
			camera.GenerateImGui();
			if (ImGui::Button("Quit"))
			{
				PostQuitMessage(0);
			}
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	graphics.Present();
}

void Application::Update(const float delta)
{
	for (auto& shape : shapes)
	{
		Vec3f& rotation = shape->rotation;
		constexpr float rate = PI / 4;

		if (window.key.KeyIsPressed('W')) rotation.x = rate;
		else if (window.key.KeyIsPressed('S')) rotation.x = -rate;
		if (window.key.KeyIsPressed('A')) rotation.z = rate;
		else if (window.key.KeyIsPressed('D')) rotation.z = -rate;
		if (window.key.KeyIsPressed('Q')) rotation.y = rate;
		else if (window.key.KeyIsPressed('E')) rotation.y = -rate;

		shape->Tick(delta);
		rotation.Clear();
	}
}