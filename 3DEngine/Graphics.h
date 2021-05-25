#pragma once

#include <d3d11.h>

class Graphics
{
	ID3D11Device* device = nullptr;
	IDXGISwapChain* swap = nullptr;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* target = nullptr;
	
public:
	Graphics(HWND window);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void Clear(float r, float g, float b) noexcept;
	void Present();
};