#pragma once
#include "Component.h"
#include "ModuleMesh.h"
#include <vector>
#include <string>

class ComponentMesh :public Component {
public:
	ModuleMesh::Mesh* mesh;
	std::string path;

	void Enable() override;
	void Update() override;
	void Disable() override;
	void EditorInspector() override;

	std::string  GetPath() const { return path; };

	void SetMesh(ModuleMesh::Mesh* mesh);
	void SetPath(std::string path);

	ComponentMesh(GameObject* parent);
};
