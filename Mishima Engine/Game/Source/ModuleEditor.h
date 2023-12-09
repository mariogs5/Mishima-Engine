#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Application.h"
#include "ModuleScene.h"
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "../Source/External/imgui/imgui.h"
#include "../Source/External/imgui/bakends/imgui_impl_sdl2.h"
#include "../Source/External/imgui/bakends/imgui_impl_opengl3.h"

#include <fstream>

#include "GameObject.h"
#include "ModuleCamera3D.h"

#include <iostream>
#include <vector>
#include <string>

#define MAX_NAME_CHARACTERS 32

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
	void Console();

	// Resources
	void UpdateFPS(const float currentFPS);
	void UpdateMS(const float currentMS);
	void UpdateDT(const float currentDT);

	//Component Inspector
	void InspectorWindow();

	//Hierarchy
	void DrawHierarchyLevel(GameObject* currentObject, int num);
	void DrawHierarchy();

	// Console
	void PrintConsole(const char file[]);

	void DrawHierarchy(std::vector<GameObject*> list);

	// Windows
	void SceneWindow();
	void GameWindow();

	// Mouse Picking
	ImVec2 NormalizeWindowPickingPoint(const float& x, const float& y, const float& w, const float& h, const ImVec2& clickPoint);

	//Fullscreen
	bool fullscreen = false;

	std::vector <std::string> consoleText;
	std::string ReadMyFile(const std::string& filename);
	std::string License;

	GameObject* GameObject_selected = nullptr;

private:

	//Inspector
	GameObject* hoveredGameObj = nullptr;
	GameObject* draggedGameObject = nullptr;

	char newName[MAX_NAME_CHARACTERS];

	bool showAddComponent = false;

	//Github Links
	void ModuleEditor::BrowserLink(const char* url);

	bool UpdateAditionalWindows = false;

	// Resources
	float currentFPS; //Current FPS
	std::vector <float> vectorFPS; //Vector of FPS
	float currentMS; //Current MS
	std::vector <float> vectorMS; //Vector of MS
	float currentDT; //Current DT
	std::vector <float> vectorDT; //Vector of DT

	// ImGui windows
	bool configWindow = true;
	bool consoleWindow = true;
	bool inspectorWindow = true;
	bool hierarchyWindow = true;
	bool sceneWindow = true;
	bool gameWindow = true;

	//Open GL settings
	bool culling;
	bool depthTest;
	bool lightCalc;
	bool colorMaterial;
	bool textureMapping;
	bool blend;
	bool normalize;
	bool wireframe = false;
	
	
};
#endif // !MODULE_EDITOR

