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
		DirectX::XMFLOAT3 pos;
		float padding;
	};
private:
	DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	mutable ShapeDrawable mesh;
	mutable PixelConstantBuffer<PointLightConstantBuffer> buffer;
};
