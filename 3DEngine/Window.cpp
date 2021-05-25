#include "Window.h"
#include <sstream>
#include <memory>

Window::WindowClass Window::WindowClass::singleton;

Window::WindowClass::WindowClass() noexcept
	:
	hInstance(GetModuleHandle(nullptr))
{
	// Register window class: https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	WNDCLASSEX clazz = {};
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

Window::Window(int width, int height, LPCWSTR name)
	:
	width(width),
	height(height)
{
	// Client window size and position
	RECT rectangle;
	rectangle.left = 100;
	rectangle.right = rectangle.left + width;
	rectangle.top = 100;
	rectangle.bottom = rectangle.top + height;
	if (AdjustWindowRect(&rectangle, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false) == 0)
	{
		throw LAST_EXCEPTION();
	}
	
	// Create the window
	window = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// Display the window
	ShowWindow(window, SW_SHOWDEFAULT);

	graphics = std::make_unique<Graphics>(window);
}

Window::~Window()
{
	DestroyWindow(window);
}

void Window::SetTitle(std::string& title)
{
	SetWindowTextA(window, title.c_str());
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return { };
}

Graphics& Window::GFX()
{
	return *graphics;
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
	// The close button was pushed
	case WM_CLOSE:
		PostQuitMessage(0);

		// We don't need to let window handle an exit, the window
		// destructor will do that for us.
		return 0;

	// Handle Keyboard buttons
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(l & 0x40000000) || key.IsAutorepeat())
		{
			key.OnKeyPressed(static_cast<unsigned char>(w));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		key.OnKeyReleased(static_cast<unsigned char>(w));
		break;
	case WM_CHAR:
		key.OnChar(static_cast<unsigned char>(w));
		break;
	case WM_KILLFOCUS:
		key.ClearState();
		break;

	// Handle Mouse clicks/movement
	case WM_MOUSEMOVE:
		const POINTS point0 = MAKEPOINTS(l);
		mouse.OnMouseMove(point0.x, point0.y);
		break;
	case WM_LBUTTONDOWN:
		const POINTS point1 = MAKEPOINTS(l);
		mouse.OnLeftPressed(point1.x, point1.y);
		break;
	case WM_RBUTTONDOWN:
		const POINTS point2 = MAKEPOINTS(l);
		mouse.OnRightPressed(point2.x, point2.y);
		break;
	case WM_LBUTTONUP:
		const POINTS point3 = MAKEPOINTS(l);
		mouse.OnLeftReleased(point3.x, point3.y);
		break;
	case WM_RBUTTONUP:
		const POINTS point4 = MAKEPOINTS(l);
		mouse.OnRightReleased(point4.x, point4.y);
		break;
	case WM_MOUSEWHEEL:
		const POINTS point5 = MAKEPOINTS(l);
		const int delta = GET_WHEEL_DELTA_WPARAM(w);
		mouse.OnWheelDelta(point5.x, point5.y, delta);
		break;
	}

	return DefWindowProc(window, msg, w, l);
}

// Window Exceptions
Window::Exception::Exception(int line, const char* file, HRESULT result) noexcept
	:
	EngineException(line, file),
	result(result)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream stream;
	stream << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();

	whatBuffer = stream.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT result)
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

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return result;
}

std::string Window::Exception::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(result);
}