#include "ComponentMesh.h"
#include "GameObject.h"


#include "External/imgui/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	mesh = nullptr;
	path = "";
	type = ComponentTypes::MESH;
};

void ComponentMesh::Enable() {}

void ComponentMesh::Disable() {}

void ComponentMesh::Update() {}

void ComponentMesh::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SetPath(std::string path)
{
	this->path = path;
}

void ComponentMesh::EditorInspector() 
{
	/*if (ImGui::CollapsingHeader("Component Mesh"))
	{
		ImGui::Text("Path: %s", paths),
			ImGui::Text("Index: %d", mesh->indices.size());
		ImGui::Text("Vertices: %d", mesh->ourVertex.size());
	}*/
}
