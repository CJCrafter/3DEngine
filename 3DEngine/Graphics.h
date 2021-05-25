#pragma once

#include <d3d11.h>
#include "EngineException.h"
#include <wrl.h>

using namespace Microsoft::WRL;  

class Graphics
{
public:
	class Exception : public EngineException
	{
		using EngineException::EngineException;
	};
	class HResultException : public Exception
	{
		HRESULT result;
		
	public:
		HResultException(int line, const char* file, HRESULT result) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetResult() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	};
	class DeviceRemovedException : public HResultException
	{
		using HResultException::HResultException;
	public:
		const char* GetType() const noexcept override;
	};
	
private:
	ComPtr<ID3D11Device>           device = nullptr;
	ComPtr<IDXGISwapChain>         swap = nullptr;
	ComPtr<ID3D11DeviceContext>    context = nullptr;
	ComPtr<ID3D11RenderTargetView> target = nullptr;
	
public:
	Graphics(HWND window);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void Clear(float r, float g, float b) noexcept;
	void DrawTriangle();
	void Present();
};
