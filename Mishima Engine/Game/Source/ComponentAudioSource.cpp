#include "ComponentAudioSource.h"
#include "ModuleAudio.h"
#include "GameObject.h"

ComponentAudioSource::ComponentAudioSource(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::AUDIOSOURCE;
	ListenerGO = parent;
	GOname = ListenerGO->name;
	id = externalapp->audio->ChangeNametoID(GOname);
}

//------------------ Default Component Funcions ------------------//
void ComponentAudioSource::Enable() {}

void ComponentAudioSource::Disable() {}

void ComponentAudioSource::Update() {}

void ComponentAudioSource::EditorInspector()
{
	if (ImGui::CollapsingHeader("Component Listener"))
	{
		ImGui::Text("GameObjectID:", id);
	}
}

//------------------ Play Audio ------------------//
void ComponentAudioSource::PlayAudio()
{
	externalapp->audio->ProcessAudio();
}