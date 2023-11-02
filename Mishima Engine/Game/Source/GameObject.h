#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/Quat.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"

#include <vector>
#include <string>


class GameObject {
public:

	GameObject(std::string name);
	~GameObject();

	bool EnableGO();
	bool DisableGO();

	void Update();

	void SetParent(GameObject* gameobject);
	GameObject* AddChildren(GameObject* children);

	Component* AddComponent(ComponentTypes component);
	Component* GetComponent(ComponentTypes type);
	

public:

	bool selected = false;
	bool active;

	std::vector<Component*> components;
	ComponentTransform* transform = nullptr;

	std::string name;
	GameObject* parent;
	std::vector<GameObject*> Children;
};
#endif // !__GAMEOBJECT_H__

