#pragma once
#include "ConstantBuffers.h"
#include "DrawableBase.h"
#include <DirectXMath.h>

template <class T>
class TransformCBuffer : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX model;
	};
public:
	TransformCBuffer(Graphics& graphics, const DrawableBase<T>& parent, UINT slot = 0)
		:
		parent(parent)
	{
		if (!buffer)
		{
			buffer = std::make_unique<VertexConstantBuffer<Transforms>>(graphics, slot);
		}
	}
	void Bind(Graphics& graphics) noexcept override
	{
		const auto model = parent.GetTransform();
		const Transforms transforms = 
		{
			DirectX::XMMatrixTranspose(model),
			DirectX::XMMatrixTranspose(
				model * graphics.GetCamera() * graphics.GetProjection()
			)
		};

		buffer->Update(graphics, transforms);
		buffer->Bind(graphics);
	}

private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> buffer;
	const DrawableBase<T>& parent;
};

template <class T>
std::unique_ptr<VertexConstantBuffer<typename TransformCBuffer<T>::Transforms>> TransformCBuffer<T>::buffer;