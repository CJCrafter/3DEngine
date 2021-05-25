#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

Graphics::Graphics(HWND window)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;                                                   // 0 allows Direct3D to look at the window and use that size
	sd.BufferDesc.Height = 0;                                                  // ^
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;                         // Layout of the channels of the pixels
	sd.BufferDesc.RefreshRate.Numerator = 0;                                   // 0 picks the default refresh rate
	sd.BufferDesc.RefreshRate.Denominator = 0;                                 //
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                     // Since we don't set width and height, no scaling will be needed
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;     //
	sd.SampleDesc.Count = 1;                                                   // We don't want anti-aliasing
	sd.SampleDesc.Quality = 0;												   // 
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                          // Where the pipeline renders to
	sd.BufferCount = 1;                                                        // Double buffering, 1 hidden buffer
	sd.OutputWindow = window;                                                  // 
	sd.Windowed = true;                                                        //
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;                                  //
	sd.Flags = 0;                                                              //
	
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swap,
		&device,
		nullptr,
		&context
	);

	ID3D11Resource* backBuffer = nullptr;
	swap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
	device->CreateRenderTargetView(
		backBuffer,
		nullptr,
		&target
	);
	backBuffer->Release();
}

Graphics::~Graphics()
{
	// It is very important to free up the system resources
	if (context != nullptr) context->Release();
	if (swap != nullptr) swap->Release();
	if (device != nullptr) device->Release();
	if (target != nullptr) target->Release();
}

void Graphics::Clear(float r, float g, float b) noexcept
{
	float color[4] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(target, color);
}

void Graphics::Present()
{
	swap->Present(1u, 0u);
}
