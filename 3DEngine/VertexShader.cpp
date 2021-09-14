#include "VertexShader.h"

#include <d3dcompiler.h>


VertexShader::VertexShader(Graphics & graphics, const std::wstring & path)
{
	GET_INFO_MANAGER(graphics);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &codeBlob));
	GFX_THROW_INFO(GetDevice(graphics)->CreateVertexShader(
		codeBlob->GetBufferPointer(),
		codeBlob->GetBufferSize(),
		nullptr,
		&vertexShader
	));
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetCode() const noexcept
{
	return codeBlob.Get();
}