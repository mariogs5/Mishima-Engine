#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "../Source/External/imgui/imgui.h"
#include "../Source/External/imgui/bakends/imgui_impl_sdl2.h"
#include "../Source/External/imgui/bakends/imgui_impl_opengl3.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	void DrawEditor();
	bool CleanUp();

	void UpdateFPS(const float aFPS);

private:

	bool UpdateAditionalWindows = false;

	//FPS LOG
	bool show_FPS_window = true;
	float aFPS; //Current FPS
	std::vector <float> mFPS; //Vector of FPS

	//Fullscreen
	bool fullscreen;
};
#endif // !MODULE_EDITOR

