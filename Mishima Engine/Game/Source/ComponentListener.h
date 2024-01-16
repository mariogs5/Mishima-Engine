#pragma once
#include "Component.h"

#include <string>

class ComponentListener :public Component {
public:

	ComponentListener(GameObject* parent);

	void Enable() override;
	void Update() override;
	void Disable() override;
	void EditorInspector() override;

public:
	GameObject* ListenerGO;
	std::string GOname;

	AkUInt32 id;
};
