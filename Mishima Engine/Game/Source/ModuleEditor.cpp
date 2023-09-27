#include "ModuleEditor.h"


ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	bool ret = true;
	return ret;

}

void ModuleEditor::DrawEditor()
{
    
}

bool ModuleEditor::CleanUp()
{	
	return true;
}
