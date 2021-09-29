#pragma once
#include "DrawableBase.h"
#include "Shape.h"
#include "Vec4.h"
#include "BindableMacro.h"

class ShapeDrawable : public DrawableBase<ShapeDrawable>
{
public:
	template<class T>
	ShapeDrawable(Graphics& graphics, const Shape<T>& shape)
	{
		if (!isStaticInitialized)
		{
			auto vertexShader = std::make_unique<VertexShader>(graphics, L"ColoredVertexShader.cso");
			auto byteCode = vertexShader->GetCode();
			AddStaticBind(std::move(vertexShader));
			AddStaticBind(std::make_unique<PixelShader>(graphics, L"PixelShader.cso"));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0}
				//{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			AddStaticBind(std::make_unique<InputLayout>(graphics, inputDesc, byteCode));
			AddStaticBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

			struct Vec4
			{
				Vec4f face_colors[6];
			};
			Vec4 colors =
			{
				{
					{Vec4f(1.0f, 0.0f, 1.0f, 0.0f)},
					{Vec4f(1.0f, 0.0f, 0.0f, 0.0f)},
					{Vec4f(0.0f, 1.0f, 0.0f, 0.0f)},
					{Vec4f(0.0f, 0.0f, 1.0f, 0.0f)},
					{Vec4f(1.0f, 1.0f, 0.0f, 0.0f)},
					{Vec4f(0.0f, 1.0f, 1.0f, 0.0f)}
				}
			};
			AddStaticBind(std::make_unique<PixelConstantBuffer<Vec4>>(graphics, colors));

			isStaticInitialized = true;
		}

		IndexedTriangleList model = shape.Geometry();
		AddBind(std::make_unique<VertexBuffer>(graphics, model.vertices));
		AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, model.indices));

		AddBind(std::make_unique<TransformCBuffer<ShapeDrawable>>(graphics, *this));
	}

};
