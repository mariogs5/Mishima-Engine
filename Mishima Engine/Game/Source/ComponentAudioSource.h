#pragma once
#include "Component.h"

#include <string>

class ComponentAudioSource :public Component {
public:

	ComponentAudioSource(GameObject* parent);

	void Enable() override;
	void Update() override;
	void Disable() override;
	void EditorInspector() override;

	void PlayAudio();

public:
	GameObject* ListenerGO;
	std::string GOname;

	AkUInt32 id;
};
