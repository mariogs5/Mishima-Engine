#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "../Source/External/MathGeoLib/include/Math/Quat.h"

#include "ComponentCamera.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// Old Camera
	/*X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 10.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	ViewMatrix = IdentityMatrix;

	CalculateViewMatrix();*/

	// New Camera
	EditorCamera = new ComponentCamera(nullptr);
	EditorCamera->SetPosition(float3(0.0f, 10.0f, 5.0f));
	EditorCamera->LookAt(float3(0.0f, 0.0f, 0.0f));
	EditorCamera->SetAspectRatio(SCREEN_WIDTH / SCREEN_HEIGHT);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	LookAt(Reference);

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

	//----------------------------- Old Camera -----------------------------//
	/*if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float3 newPos(0, 0, 0);
		speed = 3.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		Position += newPos;
		Reference += newPos;
	}*/
	

	// Mouse motion ----------------

	/*if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.35f * dt;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3 rotationAxis(0.0f, 1.0f, 0.0f);
			Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

			X = rotationQuat * X;
			Y = rotationQuat * Y;
			Z = rotationQuat * Z;
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

			Y = rotationQuat * Y;
			Z = rotationQuat * Z;

			if(Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Position = Reference + Z * Position.Length();
	}*/

	//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	//{
	//	Reference = float3(0.0f, 0.0f, 0.0f);
	//	Position = float3(0.0f, 10.0f, 5.0f);
	//	LookAt(Reference);
	//}
	//else {

	//	Reference = Position;
	//}

	/*if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		Reference = float3(0.0f, 0.0f, 0.0f);

		LookAt(Reference);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.35f * dt;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3 rotationAxis(0.0f, 1.0f, 0.0f);
			Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

			X = rotationQuat * X;
			Y = rotationQuat * Y;
			Z = rotationQuat * Z;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

			Y = rotationQuat * Y;
			Z = rotationQuat * Z;

			if (Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Position = Reference + Z * Position.Length();
	}
	else {

		Reference = Position;
	}*/

	// Zoom in & out
	
	/*if (App->input->GetMouseZ() != 0) {

		float3 newPos(0, 0, 0);
		float speed = 30.0f * dt;

		if (App->input->GetMouseZ() > 0) newPos -= Z * speed;
		if (App->input->GetMouseZ() < 0) newPos += Z * speed;

		Position += newPos;
	}*/
	
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

	// Recalculate matrix -------------
	/*CalculateViewMatrix();*/

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3&Position, const float3&Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.M;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot(Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
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