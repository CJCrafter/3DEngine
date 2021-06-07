// ReSharper disable CppClangTidyClangDiagnosticMissingBraces
#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>

#include "Window.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) HRException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if (HRESULT hr; FAILED(hr = (hrcall))) throw HRException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) HRException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) HRException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

Graphics::Graphics(HWND window)
{
	DXGI_SWAP_CHAIN_DESC sd;
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
	
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
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
	));

	ComPtr<ID3D11Resource> backBuffer = nullptr;
	GFX_THROW_INFO(swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	GFX_THROW_INFO(device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&target
	));
}

Graphics::~Graphics()
{
	// No need to free COM objects since we use ComPtr
}

void Graphics::Clear(float r, float g, float b) noexcept
{
	float color[4] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(target.Get(), color);
}

void Graphics::DrawTriangle()
{	
	struct Vertex
	{
		struct { float x, y; } position;
		struct { unsigned char r, g, b, a; } color;
	};
	const Vertex vertices[] =
	{
		// Center of octagon
		{0, 0, 255, 255, 255, 0},

		{0.0f, 0.5f, 255, 0, 0, 0},
		{0.3f, 0.25f, 255, 255, 0, 0},
		{0.3f, -0.25f, 0, 255, 0, 0},
		{0.0f, -0.5f, 0, 255, 255, 0},
		{-0.3f, -0.25f, 0, 0, 255, 0},
		{-0.3f, 0.25f, 255, 0, 255, 0}
		
	};

	ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	// Create the vertex buffer
	GFX_THROW_INFO(device->CreateBuffer(&bd, &sd, &vertexBuffer));
	

	// Bind the vertex buffer data to the pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Create the index buffer. The index buffer allows us to remove redundant vertexes.
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		4, 0, 3, 
		5, 0, 4, 
		5, 6, 0,
		0, 6, 1  
	};
	ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	device->CreateBuffer(&ibd, &isd, &indexBuffer);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	
	// Pixel shader
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3DBlob> blob;
	GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &blob));
	GFX_THROW_INFO(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
	
	// Vertex shader
	ComPtr<ID3D11VertexShader> vertexShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &blob));
	GFX_THROW_INFO(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
	context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	// Tell Direct3D how to use the Vertex structure to read points
	ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_THROW_INFO(device->CreateInputLayout(ied, std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));
	context->IASetInputLayout(inputLayout.Get());
	context->PSSetShader(pixelShader.Get(), nullptr, 0u);
	
	// Bind render target
	context->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);

	// Set primitive context (What kind of shape to draw)
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Viewports map points from the [-1, 1] coordinates to the [0, screenSize] coordinates
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 800;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1u, &vp);

	context->DrawIndexed(std::size(indices), 0u, 0u);
	//context->Draw(std::size(vertices), 0u);
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