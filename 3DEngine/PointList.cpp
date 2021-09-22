﻿#include "PointList.h"

#include "BindableMacro.h"

PointList::PointList(Graphics& graphics, const Shape& shape)
{
	if (!isStaticInitialized)
	{
		IndexedTriangleList model = shape.Generate();
		points = model.vertices.size();
		model.Transform(DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f));
		AddStaticBind(std::make_unique<VertexBuffer>(graphics, model.vertices));

		auto vertexShader = std::make_unique<VertexShader>(graphics, L"ColoredVertexShader.cso");
		auto byteCode = vertexShader->GetCode();
		AddStaticBind(std::move(vertexShader));
		AddStaticBind(std::make_unique<PixelShader>(graphics, L"ColorlessPixelShader.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0}
			//{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		AddStaticBind(std::make_unique<InputLayout>(graphics, inputDesc, byteCode));
		AddStaticBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST));

		//AddStaticBind(std::make_unique<VertexConstantBuffer<std::vector<Vertex>>>(graphics, model.vertices));

		notIndexed = true;
		isStaticInitialized = true;
	}

	AddBind(std::make_unique<TransformCBuffer<PointList>>(graphics, *this));
}

void PointList::Update(float dt) noexcept
{
}

DirectX::XMMATRIX PointList::GetTransform() const noexcept
{
	return DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}

UINT PointList::GetVertexCount() const noexcept
{
	return points;
}