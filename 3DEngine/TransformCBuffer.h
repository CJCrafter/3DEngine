﻿#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

template <class T>
class TransformCBuffer : public Bindable
{
public:
	TransformCBuffer(Graphics& graphics, const Drawable<T>& parent)
		:
		parent(parent)
	{
		if (!buffer)
		{
			buffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(graphics);
		}
	}
	void Bind(Graphics& graphics) noexcept override
	{
		buffer->Update(graphics,
			DirectX::XMMatrixTranspose(
				parent.GetTransform() * graphics.GetProjection()
			)
		);

		buffer->Bind(graphics);
	}

private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> buffer;
	const Drawable<T>& parent;
};

template <class T>
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuffer<T>::buffer;