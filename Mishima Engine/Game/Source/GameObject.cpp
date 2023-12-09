#include "GameObject.h"
#include <vector>

GameObject::GameObject(std::string name): name(name),active(true),parent(nullptr)
{
	transform = (ComponentTransform*)AddComponent(ComponentTypes::TRANSFORM);
}

// Destructor
GameObject::~GameObject()
{
	transform = nullptr;

	if (deleteGameObject && parent != nullptr) {
		parent->DeleteChild(this);
	}

	for (size_t i = 0; i < components.size(); ++i)
	{
		Component* component = components[i];
		if (component != nullptr)
		{
			delete components[i];
			components[i] = nullptr;

		}
	}

	for (size_t i = 0; i < Children.size(); ++i)
	{
		delete Children[i];
		Children[i] = nullptr;
	}

}

void GameObject::Update() 
{
	for (std::vector<Component*>::iterator co = components.begin(); co != components.end(); co++)
	{
		Component* component_update = *co;
		component_update->Update();
	}
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

bool GameObject::SetParent(GameObject* newParent)
{
	if (parent != nullptr)
	{
		if (newParent->IsChildOf(this))
		{
			return false;
		}
		parent->DeleteChild(this);

	}

	parent = newParent;
	newParent->Children.push_back(this);
	return true;
}

bool GameObject::IsChildOf(GameObject* gameobject)
{
	if (gameobject == this)
	{
		return true;
	}
	if (gameobject->Children.empty())
	{
		return false;
	}
	for (size_t i = 0; i < gameobject->Children.size(); i++)
	{
		if (IsChildOf(gameobject->Children[i]))
		{
			return true;
		}
	}
	return false;
}

void GameObject::DeleteChild(GameObject* child)
{
	for (int i = 0; i < Children.size(); i++) {
		if (Children[i] == child) {
			Children.erase(Children.begin() + i);
			child->parent = nullptr;
		}
	}
}

GameObject* GameObject::GetParent()
{
	return parent;
}

GameObject* GameObject::AddChildren(GameObject* children)
{
	children->SetParent(this);

	return children;
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
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
	case(ComponentTypes::CAMERA):
		ret = new ComponentCamera(this);
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

