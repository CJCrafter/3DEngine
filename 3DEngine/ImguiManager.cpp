#include "ImguiManager.h"
#include "imgui.h"

ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark(); // "Light theme?" "We don't do that here"
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}
