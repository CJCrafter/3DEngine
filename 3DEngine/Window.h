#pragma once
#include "EngineException.h"
#include "ModifiedWindows.h"
#include "Keyboard.h"

class Window 
{
public:
	// A Window Exception is any kind of exception that occurs with a window
	class Exception : public EngineException
	{
	public:
		Exception(int line, const char* file, HRESULT result) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT result);
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT result;
	};
private:
	class WindowClass
	{
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCWSTR className = L"My Direct3D Engine";
		static WindowClass singleton;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, LPCWSTR name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND window, UINT msg, WPARAM w, LPARAM l);
	static LRESULT CALLBACK HandleMsgThunk(HWND window, UINT msg, WPARAM w, LPARAM l);
	LRESULT HandleMsg(HWND window, UINT msg, WPARAM w, LPARAM l);

	int width;
	int height;
	HWND window;
public:
	Keyboard key;
};

// Exception location macro
#define CHWND_EXCEPT(result) Window::Exception(__LINE__, __FILE__, result)