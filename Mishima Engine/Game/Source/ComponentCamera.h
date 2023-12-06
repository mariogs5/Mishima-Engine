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


private:

	Frustum frustum;

	float verticalFOV = 60.0f;
	float horizontalFOV = 91.0f;

	float farPlane = 200.f;
	float nearPlane = 0.1f;

	float4x4 ViewMatrix = float4x4::identity;
	float4x4 ViewMatrixInverse = float4x4::identity;

};