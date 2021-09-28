#include "Texture.h"
#include "Surface.h"

Texture::Texture(Graphics& graphics, const Surface& surface)
{
	GET_INFO_MANAGER(graphics);

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = surface.GetWidth();
	textureDesc.Height = surface.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA textureData = {};
	textureData.pSysMem = surface.GetBuffer();
	textureData.SysMemPitch = surface.GetWidth() * sizeof(Color);

	ComPtr<ID3D11Texture2D> texture;
	GFX_THROW_INFO(GetDevice(graphics)->CreateTexture2D(&textureDesc, &textureData, &texture));

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = textureDesc.Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(GetDevice(graphics)->CreateShaderResourceView(texture.Get(), &desc, &textureView));
}

void Texture::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->PSSetShaderResources(0u, 1u, textureView.GetAddressOf());
}
