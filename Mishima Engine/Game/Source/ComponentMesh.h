#pragma once
#include "Component.h"
#include <vector>
#include <string>
struct Mesh;

class ComponentMesh :public Component {
public:
	Mesh* mesh;
	std::string path;

	void Enable() override;
	void Update() override;
	void Disable() override;
	void EditorInspector() override;

	Mesh* GetMesh() const { return mesh; };
	std::string  GetPath() const { return path; };

	void SetMesh(Mesh* mesh);
	void SetPath(std::string path);

	ComponentMesh(GameObject* parent);
};
