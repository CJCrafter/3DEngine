#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& graphics, const std::vector<unsigned short>& indices)
	:
	count(static_cast<UINT>(indices.size()))
{
	GET_INFO_MANAGER(graphics);

	D3D11_BUFFER_DESC indexDesc = {};
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.CPUAccessFlags = 0u;
	indexDesc.MiscFlags = 0u;
	indexDesc.ByteWidth = count * sizeof(unsigned short);
	indexDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();

	GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&indexDesc, &indexData, &indexBuffer));
}

void IndexBuffer::Bind(Graphics& graphics) noexcept
{

	GetContext(graphics)->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
