#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		:
		stride(sizeof(V))
	{
		GET_INFO_MANAGER(gfx);

		D3D11_BUFFER_DESC vertexDesc = {};
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexDesc.CPUAccessFlags = 0u;
		vertexDesc.MiscFlags = 0u;
		vertexDesc.ByteWidth = static_cast<UINT>(sizeof(V) * vertices.size());
		vertexDesc.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer));
	}
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT stride;
	ComPtr<ID3D11Buffer> vertexBuffer;
};
