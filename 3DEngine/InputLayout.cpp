#include "InputLayout.h"

InputLayout::InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	ID3DBlob* vertexShaderCode)
{
	GET_INFO_MANAGER(graphics);

	GFX_THROW_INFO(GetDevice(graphics)->CreateInputLayout(
		layout.data(), static_cast<UINT>(layout.size()),
		vertexShaderCode->GetBufferPointer(),
		vertexShaderCode->GetBufferSize(),
		&inputLayout
	));
}

void InputLayout::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->IASetInputLayout(inputLayout.Get());
}
