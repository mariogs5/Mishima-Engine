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

	show_demo_window = true;
	return ret;

}

void ModuleEditor::DrawEditor()
{
	ImGui::ShowDemoWindow(&show_demo_window);
}

bool ModuleEditor::CleanUp()
{	
	return true;
}
