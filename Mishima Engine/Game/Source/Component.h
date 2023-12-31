#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;
enum ComponentTypes 
{
	NONE,
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA,
};

class Component {
public:
	Component(GameObject* parent) :active(true), parent(parent), type(ComponentTypes::NONE) {};

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

	virtual void EditorInspector() {};

public:

	GameObject* parent;
	bool active;
	ComponentTypes type;
};

#endif // !__COMPONENT_H__

