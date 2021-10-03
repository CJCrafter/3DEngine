#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include "DirectXMath.h"
#include "imgui_impl_dx11.h"
#include "Window.h"
#include "InfoException.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND window)
{
	HRESULT hr; 

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 800;                                                   // 0 allows Direct3D to look at the window and use that size
	sd.BufferDesc.Height = 800;                                                  // ^
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
	
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		_DEBUG ? D3D11_CREATE_DEVICE_DEBUG : 0u,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swap,
		&device,
		nullptr,
		&context
	));

	ComPtr<ID3D11Resource> backBuffer = nullptr;
	GFX_THROW_INFO(swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	GFX_THROW_INFO(device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&target
	));

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	ComPtr<ID3D11DepthStencilState> depthState;
	GFX_THROW_INFO(device->CreateDepthStencilState(&depthDesc, &depthState));

	context->OMSetDepthStencilState(depthState.Get(), 1u);

	ComPtr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 800u;
	textureDesc.Height = 800u;
	textureDesc.MipLevels = 1u;
	textureDesc.ArraySize = 1u;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1u;
	textureDesc.SampleDesc.Quality = 0u;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_INFO(device->CreateTexture2D(&textureDesc, nullptr, &depthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0u;

	GFX_THROW_INFO(device->CreateDepthStencilView(depthStencil.Get(), &viewDesc, &depth));

	context->OMSetRenderTargets(1u, target.GetAddressOf(), depth.Get());

	D3D11_VIEWPORT view;
	view.Width = 800.0f;
	view.Height = 800.0f;
	view.MinDepth = 0.0f;
	view.MaxDepth = 1.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	context->RSSetViewports(1u, &view);

	ImGui_ImplDX11_Init(device.Get(), context.Get());
}

Graphics::~Graphics()
{
	// No need to free COM objects since we use ComPtr
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

void Graphics::SetProjection(DirectX::FXMMATRIX projection) noexcept
{
	this->projection = projection;
}

void Graphics::Clear(float r, float g, float b) noexcept
{
	float color[4] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(target.Get(), color);
	context->ClearDepthStencilView(depth.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(const UINT count)
{
	GFX_THROW_INFO_ONLY(context->DrawIndexed(count, 0u, 0));
}

void Graphics::Draw(const UINT count)
{
	GFX_THROW_INFO_ONLY(context->Draw(count, 0));
}

void Graphics::Present()
{
	if (HRESULT hr; FAILED(hr = swap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw DeviceRemovedException(__LINE__, __FILE__, device->GetDeviceRemovedReason());
		}
		throw GFX_EXCEPT(hr);
	}
}
