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

	// imgui Funcions
	void MainMenuBar();
	void ConfigurationWindow();

	// Resources
	void UpdateFPS(const float currentFPS);
	void UpdateMS(const float currentMS);
	void UpdateDT(const float currentDT);

	//Fullscreen
	bool fullscreen = false;

private:

	bool UpdateAditionalWindows = false;

	// Resources
	float currentFPS; //Current FPS
	std::vector <float> vectorFPS; //Vector of FPS
	float currentMS; //Current MS
	std::vector <float> vectorMS; //Vector of MS
	float currentDT; //Current DT
	std::vector <float> vectorDT; //Vector of DT

	bool configWindow = true;
	
	
};
#endif // !MODULE_EDITOR

