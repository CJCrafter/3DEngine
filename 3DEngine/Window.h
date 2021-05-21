#pragma once
#include "ModifiedWindows.h"

class Window 
{
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
	Window(int width, int height, LPCWSTR name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND window, UINT msg, WPARAM w, LPARAM l);
	static LRESULT CALLBACK HandleMsgThunk(HWND window, UINT msg, WPARAM w, LPARAM l);
	LRESULT HandleMsg(HWND window, UINT msg, WPARAM w, LPARAM l);
private:
	int width;
	int height;
	HWND window;
};