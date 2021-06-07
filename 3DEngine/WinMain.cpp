#include "Application.h"
#include "ModifiedWindows.h"
#include "Window.h"

#include "Vector3.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	Vec3f vector{3.0f, 4.0f, 5.0f};
	vector.x = 6;
	vector.y = 8;
	vector.z = 10;
	std::ostringstream stream;
	stream << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}";
	MessageBox(nullptr, stream.str().c_str(), "INFO", MB_OK);
	
	try 
	{
		return Application{}.Start();
	}
	catch (const EngineException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}