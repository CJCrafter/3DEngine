#pragma once
#include "Graphics.h"

/*
 * Bindable is an interface that wraps something that can be bound
 * to a graphics pipeline. 
 */
class Bindable
{
public:
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(Graphics& graphics) noexcept;
	static ID3D11Device* GetDevice(Graphics& graphics) noexcept;
	static DxgiInfoManager& GetInfoManager(Graphics& graphics);
};