#pragma once

#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics& graphics);
	void Bind(Graphics& graphics) noexcept override;
protected:
	ComPtr<ID3D11SamplerState> sampler;
};
