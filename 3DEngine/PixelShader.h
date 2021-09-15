#pragma once

#include "Bindable.h"

class PixelShader : public Bindable {
public:
	PixelShader(Graphics& graphics, const std::wstring& path);
	void Bind(Graphics& graphics) noexcept override;
protected:
	ComPtr<ID3D11PixelShader> pixelShader;
};