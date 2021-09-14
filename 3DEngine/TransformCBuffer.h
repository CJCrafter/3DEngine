﻿#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	TransformCbuf(Graphics& graphics, const Drawable& parent);
	void Bind(Graphics& graphics) noexcept override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> buffer;
	const Drawable& parent;
};