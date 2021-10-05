#pragma once

#include "Graphics.h"
#include "Vec3.h"

class Camera
{
public:
	Camera(float radius, float theta, float phi, float yaw, float pitch, float roll) noexcept;
	Camera(float radius, float thera, float phi, const Vec3f& angle) noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void Reset() noexcept;

	// You must call ImGui::Begin("SomeValue") before, and ImGui::End() after
	// calling this method. This method will append menu options into the currently
	// modified menu.
	void GenerateImGui();

private:
	float radius; // Distance from origin
	float theta;  // Camera positioning on sphere 
	float phi;    // Camera positioning on sphere
	Vec3f angle;  // Camera [yaw, pitch, roll] 

	// "Home" vars for when the camera is reset
	float homeRadius; // Distance from origin
	float homeTheta;  // Camera positioning on sphere 
	float homePhi;    // Camera positioning on sphere
	Vec3f homeAngle;  // Camera [yaw, pitch, roll] 
};