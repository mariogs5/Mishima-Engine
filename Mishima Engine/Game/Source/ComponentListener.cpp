#include "ComponentListener.h"
#include "ModuleAudio.h"
#include "GameObject.h"

ComponentListener::ComponentListener(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::LISTENER;
	ListenerGO = parent;
	GOname = ListenerGO->name;
	id = externalapp->audio->ChangeNametoID(GOname);
	externalapp->audio->SetDefaultListener(id);
}

void ComponentListener::Enable() {}

void ComponentListener::Disable() {}

void ComponentListener::Update() {}

void ComponentListener::EditorInspector() 
{
	if (ImGui::CollapsingHeader("Component Listener"))
	{
		ImGui::Text("GameObjectID:",id);
	}
}