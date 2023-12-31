#include "ComponentMesh.h"
#include "GameObject.h"

#include "External/imgui/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	this->parent = parent;
	mesh = nullptr;
	path = "";
	type = ComponentTypes::MESH;
};

void ComponentMesh::Enable() {}

void ComponentMesh::Disable() {}

void ComponentMesh::Update() {}

void ComponentMesh::SetMesh(ModuleMesh::Mesh* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SetPath(std::string path)
{
	this->path = path;
}

void ComponentMesh::EditorInspector() 
{
	if (ImGui::CollapsingHeader("Component Mesh"))
	{
	}
}
