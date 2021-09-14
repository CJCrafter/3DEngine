#include "PixelShader.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& graphics, const std::wstring& path)
{
	GET_INFO_MANAGER(graphics);

	ComPtr<ID3DBlob> blob;

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &blob));
	GFX_THROW_INFO(GetDevice(graphics)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
}

void PixelShader::Bind(Graphics& graphics)
{

}
