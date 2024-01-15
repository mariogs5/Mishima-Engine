#pragma once
#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ComponentCamera.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Start();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	bool CleanUp();

public:

};