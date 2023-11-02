#include "ComponentTransform.h"

#include "External/imgui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::TRANSFORM;
	UpdateMatrix();
}

ComponentTransform::ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation) : Component(parent), newScale(scale), newRotation(rotation), newPosition(position) {}

void ComponentTransform::Enable() 
{
	if (!this->active) {
		this->active = true;
	}
}
void ComponentTransform::Update() 
{

}

void ComponentTransform ::Disable()
{
	if (this->active) {
		this->active = false;
	}
}
void ComponentTransform ::EditorInspector()
{
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		ImGui::InputFloat3("Transform", (float*)&newPosition);
		ImGui::InputFloat3("Scale", (float*)&newScale);
		ImGui::InputFloat4("Rotation", (float*)&newRotation);
	}
}

void inline ComponentTransform ::SetPosition(float3 position)
{
	this->newPosition = position;  UpdateMatrix();
}

void inline ComponentTransform ::SetRotation(Quat rotation)
{
	this->newRotation = rotation; UpdateMatrix();
}

void inline ComponentTransform ::SetScale(float3 scale)
{
	this->newScale = scale; UpdateMatrix();
}

void ComponentTransform::UpdateMatrix()
{
	transform = float4x4::FromTRS(newPosition, newRotation, newScale);
}
