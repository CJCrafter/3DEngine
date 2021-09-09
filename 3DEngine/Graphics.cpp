// ReSharper disable CppClangTidyClangDiagnosticMissingBraces
#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include "DirectXMath.h"
#include "Window.h"
#include "GraphicsMacros.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND window)
{
	HRESULT hr; 

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
}

Graphics::~Graphics()
{
	// No need to free COM objects since we use ComPtr
}

void Graphics::Clear(float r, float g, float b) noexcept
{
	float color[4] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(target.Get(), color);
	context->ClearDepthStencilView(depth.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTriangle(float angle, float x, float y)
{
	// Gotta define this dude for error reporting
	HRESULT hr;

	struct Vertex
	{
		struct { float x, y, z; } position;
	};
	const Vertex vertices[] =
	{
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	};

	ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.CPUAccessFlags = 0u;
	vertexDesc.MiscFlags = 0u;
	vertexDesc.ByteWidth = sizeof(vertices);
	vertexDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices;

	// Create the vertex buffer
	GFX_THROW_INFO(device->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer));

	// Bind the vertex buffer data to the pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	// Create the index buffer. The index buffer allows us to remove redundant vertexes.
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4,
	};
	ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC indexDesc = {};
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.CPUAccessFlags = 0u;
	indexDesc.MiscFlags = 0u;
	indexDesc.ByteWidth = sizeof(indices);
	indexDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;

	device->CreateBuffer(&indexDesc, &indexData, &indexBuffer);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// Transformation Matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer matrix = 
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationX(y * 4) *
				DirectX::XMMatrixRotationY(x * 4) *
				DirectX::XMMatrixRotationZ(3.1415 / 4.0f) * 
				//DirectX::XMMatrixScaling(2 * cos * sin, 2 * cos * cos, 2 * sin) * 
				DirectX::XMMatrixTranslation(0, 0, x + 4.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 10.0)
			)
		}
	};

	ComPtr<ID3D11Buffer> matrixBuffer;
	D3D11_BUFFER_DESC matrixDesc = {};
	matrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixDesc.Usage = D3D11_USAGE_DYNAMIC; 
	matrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixDesc.MiscFlags = 0u;
	matrixDesc.ByteWidth = sizeof(matrix);
	matrixDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA matrixData = {};
	matrixData.pSysMem = &matrix;
	GFX_THROW_INFO(device->CreateBuffer(&matrixDesc, &matrixData, &matrixBuffer));
	context->VSSetConstantBuffers(0u, 1u, matrixBuffer.GetAddressOf());

	struct FaceColorBuffer
	{
		struct
		{
			float r, g, b, a;
		} face_colors[6];	
	};
	const FaceColorBuffer faceColor {
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
		}
	};
	ComPtr<ID3D11Buffer> faceColorBuffer;
	D3D11_BUFFER_DESC faceDesc = {};
	faceDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	faceDesc.Usage = D3D11_USAGE_DEFAULT;
	faceDesc.CPUAccessFlags = 0u;
	faceDesc.MiscFlags = 0u;
	faceDesc.ByteWidth = sizeof(faceColor);
	faceDesc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA faceData = {};
	faceData.pSysMem = &faceColor;
	GFX_THROW_INFO(device->CreateBuffer(&faceDesc, &faceData, &faceColorBuffer));
	context->PSSetConstantBuffers(0u, 1u, faceColorBuffer.GetAddressOf());


	GFX_THROW_INFO(device->CreateBuffer(&indexDesc, &indexData, &indexBuffer));
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
	const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	GFX_THROW_INFO(device->CreateInputLayout(inputDesc, std::size(inputDesc), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));
	context->IASetInputLayout(inputLayout.Get());
	context->PSSetShader(pixelShader.Get(), nullptr, 0u);
	
	// Set primitive context (What kind of shape to draw)
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Viewports map points from the [-1, 1] coordinates to the [0, screenSize] coordinates
	D3D11_VIEWPORT viewport;
	viewport.Width = 800;
	viewport.Height = 800;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1u, &viewport);

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