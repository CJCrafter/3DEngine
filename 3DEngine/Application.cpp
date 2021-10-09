#include "Application.h"

#include <random>

#include "IcoSphere.h"
#include "MirahCube.h"
#include "PointSphere.h"
#include "Vec3.h"
#include "GDIPlusManager.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

GDIPlusManager gdi;

Application::Application()
	:
	window(1000, 1000, "Testing testing 123"),
	timer(0.005f),
	camera(8.0f, -45.0f * 3.1415f / 180.0f, 45.0f * 3.1415f / 180.0f, {0.0f, 0.0f, 0.0f}),
	light(window.GetGraphics())
{
	std::mt19937 rand(std::random_device{}());
	std::uniform_real_distribution colorPicker(0.0f, 1.0f);
	std::uniform_real_distribution positionPicker(7.0f, 14.0f);
	std::uniform_real_distribution rotationPicker(0.0f, 3.1415f / 4.0f);

	for (int i = 0; i < 100; i++)
	{
		float color[3] = { colorPicker(rand), colorPicker(rand), colorPicker(rand) };
		shapes.push_back(std::make_unique<MirahCube>(window.GetGraphics(), color));
		auto& temp = shapes.back();

		temp->position = { positionPicker(rand), 0.0f, 0.0f };
		//temp->velocity = { c(rand), c(rand), c(rand) * 2 };
		temp->rotation = { rotationPicker(rand), rotationPicker(rand), rotationPicker(rand) };
		//temp->angle = { PI / 40.0f, PI / 40.0f, 0.0f };
		temp->scale    = Vec3f{ 1.0f, 1.0f, 1.0f } * (1 + colorPicker(rand));
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

		//if (!isPause)
		//{
			Update(delta * speed);
		//}

		Render();
	}
}

void Application::Render()
{
	Graphics& graphics = window.GetGraphics();
	graphics.Clear(0.08f, 0.01f, 0.15f);
	graphics.SetCamera(camera.GetMatrix());

	for (auto& shape : shapes)
	{
		light.Bind(graphics, camera.GetMatrix());
		shape->Draw(graphics);
	}
	light.Draw(graphics);

	if (isPause)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (false)
		{
			ImGui::ShowDemoWindow();
		}

		static bool show = true;
		if (ImGui::Begin("Debug", &show))
		{
			if (ImGui::Button("Resume"))
			{
				isPause = false;
			}
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::SliderFloat("Speed", &speed, 0.25f, 10.0f);
			if (ImGui::CollapsingHeader("Camera")) { camera.GenerateImGui(); };
			if (ImGui::CollapsingHeader("Light")) { light.GenerateImGui(); };
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

		//if (window.key.KeyIsPressed('W')) rotation.x = rate;
		//else if (window.key.KeyIsPressed('S')) rotation.x = -rate;
		//if (window.key.KeyIsPressed('A')) rotation.z = rate;
		//else if (window.key.KeyIsPressed('D')) rotation.z = -rate;
		//if (window.key.KeyIsPressed('Q')) rotation.y = rate;
		//else if (window.key.KeyIsPressed('E')) rotation.y = -rate;

		shape->Tick(delta);
		//rotation.Clear();
	}
}