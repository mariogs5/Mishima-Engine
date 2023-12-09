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

//--------------------------- Aspect Ratio ---------------------------\\

void ComponentCamera::SetAspectRatio(float AspectRatio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * AspectRatio);
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

//--------------------------- Camera Vectors ---------------------------\\

void ComponentCamera::SetZvector(float3 z)
{
	frustum.front = z;
}

void ComponentCamera::SetYvector(float3 y)
{
	frustum.up = y;
}

float3 ComponentCamera::GetXvector()
{
	return frustum.WorldRight();
}

float3 ComponentCamera::GetYvector()
{
	return frustum.up;
}

float3 ComponentCamera::GetZvector()
{
	return frustum.front;
}

//--------------------------- Camera View ---------------------------\\

void ComponentCamera::LookAt(float3& reference) 
{
	float3 Zvector = (reference - GetPosition()).Normalized();
	SetZvector(Zvector);

	float3 Xvector = float3(0, 1, 0).Cross(GetZvector()).Normalized();

	float3 Yvector = GetZvector().Cross(Xvector).Normalized();
	SetYvector(Yvector);
}

//--------------------------- Camera View ---------------------------\\

void ComponentCamera::FrustrumBox()
{
	float3 vertices[8];
	frustum.GetCornerPoints(vertices);
	externalapp->renderer3D->DrawBox(vertices, float3(255, 0, 0));
}

//--------------------------- Frame Buffer ---------------------------\\

void ComponentCamera::LoadFrameBuffer()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &TCB);
	glBindTexture(GL_TEXTURE_2D, TCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TCB, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ComponentCamera::ActivateFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ComponentCamera::DeactivateFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ComponentCamera::DestroyFrameBuffer()
{
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &TCB);
	glDeleteFramebuffers(1, &FBO);
}

//--------------------------- Inspector ---------------------------\\

void ComponentCamera::EditorInspector()
{
	if (ImGui::CollapsingHeader("Component Camera"))
	{
		ImGui::Checkbox("Frustrum Culling", &frustrumCulling);

		float* cameraPos = frustum.pos.ptr();

		ImGui::DragFloat3("Position", cameraPos, 0.1f);

		if (ImGui::DragFloat("Near plane", &nearPlane, 0.2f, 0.01f, 1000))
		{
			SetNearPlane(nearPlane);
		}

		if (ImGui::DragFloat("Far plane", &farPlane, 10, nearPlane, 10000))
		{
			SetFarPlane(farPlane);
		}
	}
}