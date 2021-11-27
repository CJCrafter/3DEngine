#pragma once

#include "Bindable.h"
#include "Vertex.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& graphics, const std::vector<V>& vertices)
		:
		stride(sizeof(V))
	{
		GET_INFO_MANAGER(graphics);

		D3D11_BUFFER_DESC vertexDesc = {};
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.CPUAccessFlags = 0u;
		vertexDesc.MiscFlags = 0u;
		vertexDesc.ByteWidth = static_cast<UINT>(sizeof(V) * vertices.size());
		vertexDesc.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer));
	}

	VertexBuffer(Graphics& graphics, const VertexHolder& vertices)
		:
		stride(static_cast<UINT>(vertices.GetLayout().Size()))
	{
		GET_INFO_MANAGER(graphics);

		D3D11_BUFFER_DESC vertexDesc = {};
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.CPUAccessFlags = 0u;
		vertexDesc.MiscFlags = 0u;
		vertexDesc.ByteWidth = static_cast<UINT>(vertices.SizeBytes());
		vertexDesc.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices.GetData();
		GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer));
	}

	void Bind(Graphics& graphics) noexcept override;

protected:
	UINT stride;
	ComPtr<ID3D11Buffer> vertexBuffer;
};
