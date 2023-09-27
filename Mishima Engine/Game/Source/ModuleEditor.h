#pragma once

#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init();
	void DrawEditor();
	bool CleanUp();

};
#endif // !MODULE_EDITOR

