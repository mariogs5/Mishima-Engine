#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled) 
{

}

ModuleAudio::~ModuleAudio(){}

bool ModuleAudio::Start()
{
	LOG("Setting up ModuleAudio");
	bool ret = true;

	return ret;
}

update_status ModuleAudio::PreUpdate(float dt) 
{

}

update_status ModuleAudio::Update(float dt) 
{

}

bool ModuleAudio::CleanUp()
{
	LOG("Cleaning ModuleAudio");

	return true;
}