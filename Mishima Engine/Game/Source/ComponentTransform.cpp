#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::TRANSFORM;
	UpdateMatrix();
}

ComponentTransform::ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation) : Component(parent), newScale(scale), newRotation(rotation), newPosition(position) {};

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
		float3 euler_rotation = newRotation.ToEulerXYZ();
		float3 euler_degree = euler_rotation * RADTODEG;
		ImGui::DragFloat3("Position:", (float*)&newPosition);
		ImGui::DragFloat3("Scale:", (float*)&newScale);
		bool rot_change = ImGui::DragFloat3("Rotation:", (float*)&euler_degree);

		if (rot_change)
		{
			euler_rotation = euler_degree * DEGTORAD;
			newRotation = Quat::FromEulerXYZ(euler_rotation.x, euler_rotation.y, euler_rotation.z);
		}

		UpdateMatrix();
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
