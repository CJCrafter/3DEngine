#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "HRException.h"
#include "DXGIInfoManager.h"

using namespace Microsoft::WRL;  

class Graphics
{
public:
	class DeviceRemovedException : public HRException
	{
		using HRException::HRException;
	};
	
private:
	ComPtr<ID3D11Device>           device = nullptr;
	ComPtr<IDXGISwapChain>         swap = nullptr;
	ComPtr<ID3D11DeviceContext>    context = nullptr;
	ComPtr<ID3D11RenderTargetView> target = nullptr;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif

public:
	Graphics(HWND window);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void Clear(float r, float g, float b) noexcept;
	void DrawTriangle();
	void Present();
};
