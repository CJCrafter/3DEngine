#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>

#include "Window.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define CHECK_FAIL(hrcall) if (FAILED(hr = (hrcall))) throw Graphics::HResultException(__LINE__, __FILE__, hr)
#define DEVICE_REMOVED(hr) throw Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))

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

	// Stores error codes for the macro.
	HRESULT hr;
	
	CHECK_FAIL(D3D11CreateDeviceAndSwapChain(
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
	CHECK_FAIL(swap->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	CHECK_FAIL(device->CreateRenderTargetView(
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
	// hr for error macro
	HRESULT hr;
	
	struct Vertex
	{
		float x, y;
	};
	const Vertex vertices[] =
	{
		{0.0f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, 0.5f},
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
	CHECK_FAIL(device->CreateBuffer(&bd, &sd, &vertexBuffer));

	// Bind the vertex buffer data to the pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Pixel shader
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(L"PixelShader.hlsl", &blob);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	
	// Vertex shader
	ComPtr<ID3D11VertexShader> vertexShader;
	D3DReadFileToBlob(L"VertexShader.hlsl", &blob);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
	context->VSSetShader(vertexShader.Get(), nullptr, 0u);

	// Tell Direct3D how to use the Vertex structure to read points
	ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	device->CreateInputLayout(ied, std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	context->IASetInputLayout(inputLayout.Get());
	context->PSSetShader(pixelShader.Get(), nullptr, 0u);
	
	// Bind render target
	context->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);

	// Set primitive context (What kind of shape to draw)
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Viewports map points from the [-1, 1] coordinates to the [0, screenSize] coordinates
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1u, &vp);
	
	context->Draw(std::size(vertices), 0u);
}

void Graphics::Present()
{
	// Stores error codes for the macro
	HRESULT hr;
	
	if (FAILED(hr = swap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			hr = device->GetDeviceRemovedReason();
			DEVICE_REMOVED(device->GetDeviceRemovedReason());
		}
		CHECK_FAIL(hr);
	}
}

Graphics::HResultException::HResultException(int line, const char* file, HRESULT result) noexcept
	:
	Exception(line, file),
	result(result)
{}

const char* Graphics::HResultException::what() const noexcept
{
	std::ostringstream stream;
	stream << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetResult()
		<< std::dec << " (" << static_cast<unsigned long>(GetResult()) << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();

	std::string buffer = stream.str();
	return buffer.c_str();
}

const char* Graphics::HResultException::GetType() const noexcept
{
	return "Graphics Exception";
}

HRESULT Graphics::HResultException::GetResult() const noexcept
{
	return result;
}

std::string Graphics::HResultException::GetErrorString() const noexcept
{
	return "I am too lazy to implement this";
}

std::string Graphics::HResultException::GetErrorDescription() const noexcept
{
	char* buffer = nullptr;
	DWORD msg = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&buffer),
		0, nullptr
	);
	if (msg == 0)
	{
		return "Unexpected error code";
	}
	std::string error = buffer;
	LocalFree(buffer);
	return error;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Device Removed Exception";
}
