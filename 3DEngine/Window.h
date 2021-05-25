#pragma once
#include "EngineException.h"
#include "ModifiedWindows.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <sstream>
#include <optional>

#include "Graphics.h"

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

	void SetTitle(std::string& title);
	static std::optional<int> ProcessMessages();
	Graphics& GFX();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND window, UINT msg, WPARAM w, LPARAM l);
	static LRESULT CALLBACK HandleMsgThunk(HWND window, UINT msg, WPARAM w, LPARAM l);
	LRESULT HandleMsg(HWND window, UINT msg, WPARAM w, LPARAM l);

	int width;
	int height;
	HWND window;
	std::unique_ptr<Graphics> graphics;

public:
	Keyboard key;
	Mouse mouse;
	
};

// Exception location macro
#define EXCEPTION(result) Window::Exception(__LINE__, __FILE__, result)
#define LAST_EXCEPTION() Window::Exception(__LINE__, __FILE__, GetLastError())