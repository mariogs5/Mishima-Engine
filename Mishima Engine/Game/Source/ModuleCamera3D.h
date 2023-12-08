#pragma once
#include "Module.h"
#include "Globals.h"
#include "../Source/External/MathGeoLib/include/Math/float3.h"
#include "../Source/External/MathGeoLib/include/Math/float4x4.h"

#include "GameObject.h"
#include "ComponentCamera.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

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
	float* GetViewMatrix();

	//-------- Camera Motion --------//
	void fpsMovement(ComponentCamera* camera, float3 newPos, float speed);
	void CameraRotation(ComponentCamera* camera, float dt);
	void CameraZoom(ComponentCamera* camera, float3 newPos, float speed);

private:

	void CalculateViewMatrix();

public:

	float speed;

	//-------- New Camera --------//
	ComponentCamera* EditorCamera;
	

	//You won't need this after using Frustum
	float3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix;
	//Frustum mMainCamera; Some help here :)
};