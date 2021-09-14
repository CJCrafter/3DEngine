#pragma once
#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& graphics,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* vertexShaderCode);
	void Bind(Graphics& graphics) noexcept override;
protected:
	ComPtr<ID3D11InputLayout> inputLayout;
};
