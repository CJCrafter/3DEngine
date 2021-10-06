#include "PointLight.h"

#include "imgui.h"

PointLight::PointLight(Graphics& graphics, float radius)
	:
	mesh(graphics, IcoSphere<VertexBase>(1)),
	buffer(graphics)
{
}

void PointLight::GenerateImGui() noexcept
{
	ImGui::Text("Light");
	ImGui::SliderFloat("X", &pos.x, -60.0f, +60.0f, "%.1f");
	ImGui::SliderFloat("Y", &pos.y, -60.0f, +60.0f, "%.1f");
	ImGui::SliderFloat("Z", &pos.z, -60.0f, +60.0f, "%.1f");
	if (ImGui::Button("Reset"))
	{
		Reset();
	}
}

void PointLight::Reset() noexcept
{
	pos = { 0.0f, 0.0f, 0.0f };
}

void PointLight::Draw(Graphics& graphics) const
{
	mesh.position = pos;
	mesh.Draw(graphics);
}

void PointLight::Bind(Graphics& graphics) const noexcept
{
	buffer.Update(graphics, PointLightConstantBuffer{ pos });
}
