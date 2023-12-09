#pragma once
#include "Module.h"
#include "Globals.h"
#include "../Source/External/MathGeoLib/include/Math/float3.h"
#include "../Source/External/MathGeoLib/include/Math/float4x4.h"

#include "GameObject.h"

class ComponentCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3&Reference, bool RotateAroundReference = false);
	void LookAt(const float3&Spot);
	void Move(const float3&Movement);

	//-------- Camera Motion --------//
	void fpsMovement(ComponentCamera* camera, float3& newPos, float speed);
	void CameraRotation(ComponentCamera* camera, float dt);
	void CameraZoom(ComponentCamera* camera, float3& newPos, float speed);

	//-------- Mouse Picking --------//
	void MousePicking(float mouseXNorm, float mouseYNorm);
	LineSegment MouseRay;

public:

	float speed;

	//-------- New Camera --------//
	ComponentCamera* EditorCamera;
};