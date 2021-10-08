#pragma once

#include "Graphics.h"
#include "IcoSphere.h"
#include "ConstantBuffers.h"
#include "ShapeDrawable.h"

class PointLight
{
public:
	PointLight(Graphics& graphics, float radius = 0.5f);
	void GenerateImGui() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& graphics) const;
	void Bind(Graphics& graphics) const noexcept;
private:
	struct PointLightConstantBuffer
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambientColor;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightConstantBuffer data;
	mutable ShapeDrawable mesh;
	mutable PixelConstantBuffer<PointLightConstantBuffer> buffer;
};
