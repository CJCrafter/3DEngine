#include "Camera.h"
#include "Math.h"
#include "imgui.h"
#include "ImguiManager.h"

Camera::Camera(float radius, float theta, float phi, float yaw, float pitch, float roll) noexcept
	: 
	radius(radius),
	theta(theta),
	phi(phi),
	angle({yaw, pitch, roll}),
	homeRadius(radius),
	homeTheta(theta),
	homePhi(phi),
	homeAngle({ yaw, pitch, roll })
{
}

Camera::Camera(float radius, float theta, float phi, const Vec3f& angle) noexcept
	: 
	radius(radius),
	theta(theta),
	phi(phi),
	angle(Vec3f(angle)),
	homeRadius(radius),
	homeTheta(theta),
	homePhi(phi),
	homeAngle(Vec3f(angle))
{
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto pos = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, -radius, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);
	return DirectX::XMMatrixLookAtLH(
		pos, DirectX::XMVectorZero(),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * DirectX::XMMatrixRotationRollPitchYaw(
		angle.y, -angle.x, angle.z
	);
}

void Camera::Reset() noexcept
{
	radius = homeRadius;
	theta = homeTheta;
	phi = homePhi;
	angle = homeAngle;
}

void Camera::GenerateImGui()
{
	ImGui::Text("Camera");
	ImGui::SliderFloat("Distance", &radius, 0.1f, 40.0f);
	ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
	ImGui::SliderAngle("Phi", &phi, -180.0f, 180.0f);
	ImGui::SliderAngle("Yaw", &angle.x, -180.0f, 180.0f);
	ImGui::SliderAngle("Pitch", &angle.y, -180.0f, 180.0f);
	ImGui::SliderAngle("Roll", &angle.z, -180.0f, 180.0f);

	if (ImGui::Button("Reset")) 
	{
		Reset();
	}
}
