#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& graphics) noexcept
{
	return graphics.context.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& graphics) noexcept
{
	return graphics.device.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx)
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error("Info manager cannot be used in release, please switch to debug");
#endif
}