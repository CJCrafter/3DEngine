#include "PointLight.h"

#include "imgui.h"

PointLight::PointLight(Graphics& graphics, float radius)
	:
	mesh(graphics, IcoSphere<VertexBase>(1)),
	buffer(graphics)
{
	mesh.scale = {radius, radius, radius};
	Reset();
}

void PointLight::GenerateImGui() noexcept
{
	ImGui::Text("Position");
	ImGui::SliderFloat("X", &data.pos.x, -20.0f, +20.0f, "%.2f");
	ImGui::SliderFloat("Y", &data.pos.y, -20.0f, +20.0f, "%.2f");
	ImGui::SliderFloat("Z", &data.pos.z, -20.0f, +20.0f, "%.2f");

	ImGui::Text("Color");
	ImGui::ColorEdit3("Material", &data.materialColor.x);
	ImGui::ColorEdit3("Ambient", &data.ambientColor.x);
	ImGui::ColorEdit3("Color", &data.diffuseColor.x);
	ImGui::SliderFloat("Intensity", &data.diffuseIntensity, 0.01f, 2.0f);
	
	ImGui::Text("Dropoff");
	ImGui::SliderFloat("Constant", &data.attConst, 0.001f, 5.0f, "%.2f");
	ImGui::SliderFloat("Linear", &data.attLin, 0.0f, 2.0f, "%.4f");
	ImGui::SliderFloat("Quadratic", &data.attQuad, 0.0f, 1.0f, "%.8f");

	if (ImGui::Button("Reset"))
	{
		Reset();
	}
}

void PointLight::Reset() noexcept
{
	data.pos = { 0.0f, 0.0f, 0.0f };
	data.materialColor = { 0.7f, 0.7f, 0.9f };
	data.ambientColor = { 0.05f, 0.05f, 0.05f };
	data.diffuseColor = { 1.0f, 1.0f, 1.0f };
	data.diffuseIntensity = 1.0f;
	data.attConst = 1.0f;
	data.attLin = 0.045f;
	data.attQuad = 0.0075f;
}

void PointLight::Draw(Graphics& graphics) const
{
	mesh.position = data.pos;
	mesh.Draw(graphics);
}

void PointLight::Bind(Graphics& graphics) const noexcept
{
	buffer.Update(graphics, PointLightConstantBuffer{ data });
	buffer.Bind(graphics);
}
