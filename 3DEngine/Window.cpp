#include "Window.h"

Window::WindowClass Window::WindowClass::singleton;

Window::WindowClass::WindowClass() noexcept
	:
	hInstance(GetModuleHandle(nullptr))
{
	// Register window class: https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	WNDCLASSEX clazz = { 0 };
	clazz.cbSize = sizeof(clazz);
	clazz.style = CS_OWNDC;
	clazz.lpfnWndProc = HandleMsgSetup;
	clazz.cbClsExtra = 0;
	clazz.cbWndExtra = 0;
	clazz.hInstance = GetInstance();
	clazz.hIcon = nullptr;
	clazz.hCursor = nullptr;
	clazz.hbrBackground = nullptr;
	clazz.lpszMenuName = nullptr;
	clazz.lpszClassName = className;
	clazz.hIconSm = nullptr;
	RegisterClassEx(&clazz);
}

Window::WindowClass::~WindowClass() 
{
	UnregisterClass(className, GetInstance());
}

LPCWSTR Window::WindowClass::GetName() noexcept
{
	return className;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return singleton.hInstance;
}

Window::Window(int width, int height, LPCWSTR name) noexcept
{
	// Client window size and position
	RECT rectangle;
	rectangle.left = 100;
	rectangle.right = rectangle.left + width;
	rectangle.top = 100;
	rectangle.bottom = rectangle.top + height;
	AdjustWindowRect(&rectangle, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

	// Create the window
	window = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// Display the window
	ShowWindow(window, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(window);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND window, UINT msg, WPARAM w, LPARAM l)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(l);
		const auto win = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return win->HandleMsg(window, msg, w, l);
	}
	return DefWindowProc(window, msg, w, l);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND window, UINT msg, WPARAM w, LPARAM l)
{
	const auto win = reinterpret_cast<Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
	return win->HandleMsg(window, msg, w, l);
}

LRESULT Window::HandleMsg(HWND window, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(window, msg, w, l);
}