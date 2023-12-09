#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/Quat.h"
#include "Application.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentCamera.h"

#include <vector>
#include <string>


class GameObject {
public:

	GameObject(std::string name);
	~GameObject();

	bool EnableGO();
	bool DisableGO();

	void Update();

	bool SetParent(GameObject* newParent);
	bool IsChildOf(GameObject* parent);
	void DeleteChild(GameObject* child);

	Component* AddComponent(ComponentTypes component);
	void AddComponent(Component* component);
	Component* GetComponent(ComponentTypes type);
	GameObject* AddChildren(GameObject* children);
	GameObject* GetParent();
	

public:

	std::string name;

	GameObject* parent;
	std::vector<GameObject*> Children;

	bool selected = false;
	bool active = true;
	bool deleteGameObject;

	std::vector<Component*> components;
	ComponentTransform* transform = nullptr;
};
#endif // !__GAMEOBJECT_H__

