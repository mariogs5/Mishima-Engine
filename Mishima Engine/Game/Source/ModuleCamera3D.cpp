#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "../Source/External/MathGeoLib/include/Math/Quat.h"

#include "ModuleScene.h"
#include "ComponentCamera.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// New Camera
	EditorCamera = new ComponentCamera(nullptr);
	EditorCamera->SetPosition(float3(0.0f, 10.0f, 5.0f));
	EditorCamera->LookAt(float3(0.0f, 0.0f, 0.0f));
	EditorCamera->SetAspectRatio((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	//----------------------------- New Camera -----------------------------//

	float3 newPos(0, 0, 0);
	speed = 10.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 20.0f * dt;

	//-------- Camera Rotation --------//
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		CameraRotation(EditorCamera, dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		//EditorCamera->LookAt(float3(0.0f, 0.0f, 0.0f));
		CameraRotation(EditorCamera, dt);
	}
	//-------- Camera Zoom --------//
	if (App->input->GetMouseZ() != 0)
	{
		CameraZoom(EditorCamera, newPos, speed);
	}

	//-------- FPS Movement --------//
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		fpsMovement(EditorCamera, newPos, speed);
		
	}
	EditorCamera->MoveCamera(newPos);

	//-------- Center Camera --------//
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) 
	{
		EditorCamera->LookAt(float3(0.0f, 0.0f, 0.0f));
		EditorCamera->SetPosition(float3(0.0f, 10.0f, 5.0f));
	}

	// 2D camera (not working)

	/*if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		float3 newPos(0, 0, 0);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float speed = 3.0f * dt;

		if (dx != 0) 
		{
			float DeltaX = (float)dx * speed;
			newPos.x += DeltaX;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * speed;
			newPos.y += DeltaY;
		}

		Position += newPos;
	}*/

	return UPDATE_CONTINUE;
}

//-------- Mouse Picking --------//
void ModuleCamera3D::MousePicking(float mouseXNorm, float mouseYNorm)
{
	MouseRay = EditorCamera->frustum.UnProjectLineSegment(mouseXNorm, mouseYNorm);
	App->scene->GameObjectPicking(MouseRay);
}

//----------------------------- New Camera -----------------------------//

void ModuleCamera3D::fpsMovement(ComponentCamera* camera, float3& newPos, float speed)
{
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos += camera->GetYvector() * speed;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos -= camera->GetYvector() * speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += camera->GetZvector() * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= camera->GetZvector() * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= camera->GetXvector() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += camera->GetXvector() * speed;
}

void ModuleCamera3D::CameraRotation(ComponentCamera* camera, float dt)
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.35f * dt;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;

		float3 rotationAxis(0.0f, 1.0f, 0.0f);
		Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

		camera->SetYvector(rotationQuat * camera->GetYvector());
		camera->SetZvector(rotationQuat * camera->GetZvector());
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * Sensitivity;

		Quat rotationQuat = Quat::RotateAxisAngle(camera->GetXvector(), DeltaY);

		camera->SetYvector(rotationQuat * camera->GetYvector());
		camera->SetZvector(rotationQuat * camera->GetZvector());

		if (camera->GetYvector().y < 0.0f)
		{
			camera->SetZvector(float3(0.0f, camera->GetZvector().y > 0.0f ? 1.0f : -1.0f, 0.0f));
			camera->SetYvector(camera->GetZvector().Cross(camera->GetXvector()));
		}
	}
}

void ModuleCamera3D::CameraZoom(ComponentCamera* camera, float3& newPos, float speed)
{
	if (App->input->GetMouseZ() > 0) newPos += camera->GetZvector() * speed;
	if (App->input->GetMouseZ() < 0) newPos -= camera->GetZvector() * speed;
}