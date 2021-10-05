#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "HRException.h"
#include "DXGIInfoManager.h"
#include "GraphicsMacros.h"

using namespace Microsoft::WRL;  

class Graphics
{
	friend class Bindable;
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
	ComPtr<ID3D11DepthStencilView> depth = nullptr;

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif

public:
	Graphics(HWND window);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetProjection(DirectX::FXMMATRIX projection) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
	void SetCamera(DirectX::FXMMATRIX camera) noexcept;

	void Clear(float r, float g, float b) noexcept;
	void DrawIndexed(UINT count);
	void Draw(UINT count);
	void Present();
};
