#include "ModifiedWindows.h"
#include "Window.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	Window window(800, 300, L"Testing Testing 123");

	// Message pump. We want to handle messages until an error occurs, or the application is quit
	MSG msg;
	BOOL result;
	while ((result = GetMessage(&msg, nullptr, 0, 0)) > 0) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return result < 0 ? -1 : msg.wParam;
}