#include "GameObject.h"
#include <vector>

GameObject::GameObject (std::string name)
{
}

// Destructor
GameObject::~GameObject()
{}

void GameObject::Update() 
{

}

bool GameObject::EnableGO()
{
    if (!active) 
    {
        active = true;
        return true;
    }
    return false;
}

bool GameObject::DisableGO()
{
    if (active) 
    {
        active = false;
        return true;
    }
    return false;
}

void GameObject::SetParent(GameObject* gameobject)
{
    this->parent = gameobject;
}

GameObject* GameObject::AddChildren(GameObject* children) 
{
    if (Children.empty()) 
    {
        Children.push_back(children);
    }
    return children;
}

Component* GameObject::AddComponent(ComponentTypes component) 
{

	Component* ret = nullptr;

	switch (component)
	{
	case(ComponentTypes::NONE):
		break;
	case(ComponentTypes::TRANSFORM):
		ret = new ComponentTransform(this);
		break;
	case(ComponentTypes::TEXTURE):
		if (this->GetComponent(ComponentTypes::TEXTURE) != nullptr) {
			return nullptr;
		}
		ret = new ComponentTexture(this);
		break;
	case(ComponentTypes::MESH):
		ret = new ComponentMesh(this);
		break;

	}
	components.push_back(ret);
	return ret;
}

Component* GameObject::GetComponent(ComponentTypes type)
{
	std::vector<Component*>::iterator item = components.begin();

	for (; item != components.end(); ++item) {

		if ((*item)->type == type) {
			return (*item);
		}
	}

	return nullptr;
}

