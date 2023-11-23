#pragma once
#include "Component.h"

struct Texture;

class ComponentTexture : public Component 
{
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void EditorInspector() override;

	void SetTexture(Texture* texture);
	Texture* GetTexture();

	ComponentTexture(GameObject* parent);
	Texture* texture = nullptr;
};