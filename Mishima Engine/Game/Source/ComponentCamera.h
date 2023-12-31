#pragma once
#include "Component.h"
#include "ModuleCamera3D.h"
#include "Application.h"
#include "ModuleWindow.h"

#include "External/MathGeoLib/include/MathGeoLib.h"
#include "External/MathGeoLib/include/MathBuildConfig.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float4x4.h"


class ComponentCamera : public Component {

public:
	ComponentCamera(GameObject* parent);
	void Enable() override;
	void Update() override;
	void Disable() override;
	void EditorInspector() override;

	//-------- Matrix --------//
	float4x4 GetProjectionMatrix() const;
	float4x4 GetViewMatrix() const;

	//-------- Vertical FOV --------//
	void SetVerticalFov(float vertical_fov);
	float GetVerticalFov() const;

	//-------- Aspect Ratio --------//
	void SetAspectRatio(float AspectRatio);

	//-------- Horizontal FOV --------//
	void SetHorizontalFov(float horitzontal_fov);
	float GetHorizontalFov() const;

	//-------- Far Plane --------//
	void SetFarPlane(float far_plane);
	float GetFarPlane() const;

	//-------- Near Plane --------//
	void SetNearPlane(float near_plane);
	float GetNearPlane() const;

	//-------- Camera Position --------//
	void SetPosition(const float3 position);
	float3 GetPosition() const;
	void MoveCamera(float3 newPos);

	//-------- Camera Vectors --------//
	void SetZvector(float3 z);
	void SetYvector(float3 y);

	float3 GetXvector();
	float3 GetZvector();
	float3 GetYvector();

	//-------- Camera View --------//
	void LookAt(float3& reference);

	//-------- Frame Buffer --------//
	void LoadFrameBuffer();
	void ActivateFrameBuffer();
	void DeactivateFrameBuffer();
	void DestroyFrameBuffer();

	GLuint FBO;
	GLuint TCB;
	GLuint RBO;

	//-------- Frustrum Box --------//
	void FrustrumBox();

	Frustum frustum;

	bool frustrumCulling = true;

private:

	float verticalFOV = 60.0f;

	float farPlane = 1000.f;
	float nearPlane = 1.0f;

	float4x4 ViewMatrix = float4x4::identity;
	float4x4 ViewMatrixInverse = float4x4::identity;

};