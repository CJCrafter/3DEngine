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
private:
	class WindowClass
	{
	public:
		static LPCSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCSTR className = "My Direct3D Engine";
		static WindowClass singleton;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, LPCSTR name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	int GetWidth()
	{
		return width;
	}

	int GetHeight()
	{
		return height;
	}

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