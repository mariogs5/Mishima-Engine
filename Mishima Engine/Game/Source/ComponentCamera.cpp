#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Color.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = ComponentTypes::CAMERA;
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = nearPlane;
	frustum.farPlaneDistance = farPlane;
	frustum.verticalFov = DEGTORAD * verticalFOV;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);
}

void ComponentCamera::Enable()
{}

void ComponentCamera::Disable()
{}

void ComponentCamera::Update()
{
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)GetProjectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)GetViewMatrix().v);
}

//--------------------------- Matrix ---------------------------\\

float4x4 ComponentCamera::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed();
}

float4x4 ComponentCamera::GetViewMatrix() const
{
	float4x4 viewMatrix = frustum.ViewMatrix();
	return viewMatrix.Transposed();
}

//--------------------------- Camera Pos ---------------------------\\

void ComponentCamera::SetPosition(float3 position) 
{
	frustum.pos = position;
}

float3 ComponentCamera::GetPosition() const
{
	return frustum.pos;
}

void ComponentCamera::MoveCamera(float3 newPos) 
{
	frustum.pos += newPos;
}

//--------------------------- Near Plane ---------------------------\\

void ComponentCamera::SetNearPlane(float near_plane) 
{
	frustum.nearPlaneDistance = near_plane;
}

float ComponentCamera::GetNearPlane() const
{
	return frustum.nearPlaneDistance;
}

//--------------------------- Far Plane ---------------------------\\

void ComponentCamera::SetFarPlane(float far_plane) 
{
	frustum.farPlaneDistance = far_plane;
}

float ComponentCamera::GetFarPlane() const
{
	return frustum.farPlaneDistance;
}

//--------------------------- Horizontal FOV ---------------------------\\

void ComponentCamera::SetHorizontalFov(float horizontal_fov) 
{
	frustum.horizontalFov = horizontal_fov;
}

float ComponentCamera::GetHorizontalFov() const 
{
	return frustum.horizontalFov;
}

//--------------------------- Vertical FOV ---------------------------\\

void ComponentCamera::SetVerticalFov(float vertical_fov)
{
	frustum.verticalFov = vertical_fov;
}

float ComponentCamera::GetVerticalFov() const
{
	return frustum.verticalFov;
}

//--------------------------- Inspector ---------------------------\\

void ComponentCamera::EditorInspector()
{
	if (ImGui::CollapsingHeader("Component Camera"))
	{
		if (ImGui::DragFloat("Near plane", &nearPlane, 0.2f, 0.01f, 1000))
		{
			SetNearPlane(nearPlane);
		}

		if (ImGui::DragFloat("Far plane", &farPlane, 10, nearPlane, 10000))
		{
			SetFarPlane(farPlane);
		}

		/*if (ImGui::DragFloat("Field of view", &fov, 1, 1, 179.9f))
		{
			camera->SetFOV(fov);
		}*/
	}
}