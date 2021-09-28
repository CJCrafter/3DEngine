#include "Sampler.h"

Sampler::Sampler(Graphics& graphics)
{
	GET_INFO_MANAGER(graphics);

	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(GetDevice(graphics)->CreateSamplerState(&desc, &sampler));
}

void Sampler::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->PSSetSamplers(0, 1, sampler.GetAddressOf());
}
