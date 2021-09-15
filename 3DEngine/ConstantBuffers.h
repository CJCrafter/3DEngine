#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& graphics, const C& constants)
	{
		GET_INFO_MANAGER(graphics);

		D3D11_MAPPED_SUBRESOURCE mappedData;
		GFX_THROW_INFO(GetContext(graphics)->Map(
			constantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&mappedData
		));
		memcpy(mappedData.pData, &constants, sizeof(constants));
		GetContext(graphics)->Unmap(constantBuffer.Get(), 0u);
	}
	ConstantBuffer(Graphics& graphics, const C& constants)
	{
		GET_INFO_MANAGER(graphics);

		D3D11_BUFFER_DESC constantDesc;
		constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantDesc.MiscFlags = 0u;
		constantDesc.ByteWidth = sizeof(constants);
		constantDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA constantData = {};
		constantData.pSysMem = &constants;
		GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&constantDesc, &constantData, &constantBuffer));
	}
	ConstantBuffer(Graphics& graphics)
	{
		GET_INFO_MANAGER(graphics);

		D3D11_BUFFER_DESC constantDesc;
		constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantDesc.MiscFlags = 0u;
		constantDesc.ByteWidth = sizeof(C);
		constantDesc.StructureByteStride = 0u;
		GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&constantDesc, nullptr, &constantBuffer));
	}
protected:
	ComPtr<ID3D11Buffer> constantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		this->GetContext(graphics)->VSSetConstantBuffers(0u, 1u, this->constantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& graphics) noexcept override
	{
		this->GetContext(graphics)->PSSetConstantBuffers(0u, 1u, this->constantBuffer.GetAddressOf());
	}
};