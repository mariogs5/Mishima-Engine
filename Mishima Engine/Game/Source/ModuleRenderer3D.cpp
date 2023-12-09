#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleMesh.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Game/Source/External/Glew/libx86/glew32.lib")

#ifdef _DEBUG
#pragma comment (lib, "Game/Source/External/MathGeoLib/libx86/MathGeoLibDebug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "Game/Source/External/MathGeoLib/libx86/MathGeoLibRelease/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Setup Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glewInit();

		//Init Devil
		InitDevil();
	}

	ilInit();

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkTexture);
	glBindTexture(GL_TEXTURE_2D, checkTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	App->scene->LoadMeshAndTexture("Assets/Primitives/BakerHouse.fbx", "Assets/Primitives/Baker_House.png");
	//App->scene->LoadMeshAndTexture("Assets/Primitives/street.fbx", "Assets/Primitives/Baker_House.png");
	//App->mesh->LoadMesh("Assets/Primitives/street.fbx");
	//App->texture->LoadTexture("Assets/Primitives/Baker_House.png");
	BindBuffers();

	gameObject_list = App->scene->GetGameObjects();

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Grid.axis = true;

	//-------- Bounding Boxes --------//
	App->mesh->InitBoundingBoxes();

	//-------- Frame Buffer --------//
	App->camera->EditorCamera->LoadFrameBuffer();
	App->scene->gameCameraComponent->LoadFrameBuffer();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// light 0 on cam pos
	//lights[0].SetPos(App->camera->Position.z, App->camera->Position.z, App->camera->Position.z);
	lights[0].SetPos(App->camera->EditorCamera->GetPosition().x, App->camera->EditorCamera->GetPosition().y, App->camera->EditorCamera->GetPosition().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	App->camera->EditorCamera->Update();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->camera->EditorCamera->ActivateFrameBuffer();
	App->camera->EditorCamera->Update();

	App->scene->gameCameraComponent->FrustrumBox();

	DrawGameObjects();
	
	if (activeNormals)
	{
		App->mesh->DrawNormals();
	}

	Grid.Render();

	App->camera->EditorCamera->DeactivateFrameBuffer();

	App->scene->gameCameraComponent->ActivateFrameBuffer();
	App->scene->gameCameraComponent->Update();

	DrawGameObjects();

	if (activeNormals)
	{
		App->mesh->DrawNormals();
	}

	App->scene->gameCameraComponent->DeactivateFrameBuffer();

	App->editor->DrawEditor();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	for (int i = 0; i < App->mesh->ourMeshes.size(); i++) {

		glDeleteBuffers(1, &App->mesh->ourMeshes[i]->VBO);
		glDeleteBuffers(1, &App->mesh->ourMeshes[i]->EBO);
	}

	//Models.clear();
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_GL_DeleteContext(context);

	App->camera->EditorCamera->DestroyFrameBuffer();
	App->scene->gameCameraComponent->DestroyFrameBuffer();

	return true;
}

void ModuleRenderer3D::InitDevil()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	LOG("Devil has initialised successfully");
}

void ModuleRenderer3D::DrawBox(float3* vertices, float3 color)
{
	uint indices[24] = {
		0,2,2,
		6,6,4,
		4,0,0,
		1,1,3,
		3,2,4,
		5,6,7,
		5,7,3,
		7,1,5
	};
	glBegin(GL_LINES);
	glColor3fv(color.ptr());

	for (size_t i = 0; i < (sizeof(indices) / sizeof(indices[0])); i++) {

		glVertex3fv(vertices[indices[i]].ptr());

	}

	glColor3f(255.f, 255.f, 255.f);
	glEnd();
}

bool ModuleRenderer3D::InsideFrustrum(const ComponentCamera* camera, const AABB& aabb)
{
	if (camera->frustrumCulling) 
	{
		Plane frustumPlanes[6];
		camera->frustum.GetPlanes(frustumPlanes);

		float3 cornerPoints[8];
		aabb.GetCornerPoints(cornerPoints);

		for (int i = 0; i < 6; ++i) {

			uint pointsInside = 8;

			for (int j = 0; j < 8; ++j)
			{
				if (frustumPlanes[i].IsOnPositiveSide(cornerPoints[j]))
				{
					--pointsInside;
				}
			}

			if (pointsInside == 0)
			{
				return false;
			}
		}
	}

	return true;
}

void ModuleRenderer3D::BindBuffers()
{
	//Todo: Hacer binnd de todos los meshes arreglar
	for (int i = 0; i < App->mesh->ourMeshes.size(); i++) {

		glGenBuffers(1, &App->mesh->ourMeshes[i]->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, App->mesh->ourMeshes[i]->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ModuleMesh::Vertex) * App->mesh->ourMeshes[i]->ourVertex.size(), &App->mesh->ourMeshes[i]->ourVertex[0], GL_STATIC_DRAW);

		glGenBuffers(1, &App->mesh->ourMeshes[i]->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->mesh->ourMeshes[i]->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * App->mesh->ourMeshes[i]->indices.size(), &App->mesh->ourMeshes[i]->indices[0], GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void ModuleRenderer3D::DrawGameObjects() 
{
	for (uint n = 0; n < gameObject_list.size(); n++)
	{
		GameObject* gameobject = gameObject_list[n];

		if (!gameobject->active)
		{
			continue;
		}

		for (uint m = 0; m < gameobject->components.size(); m++)
		{
			Component* component = gameobject->components[m];
		
			if (component->type != ComponentTypes::MESH)
			{
				continue;
			}

			ComponentMesh* componentMesh = (ComponentMesh*)component;

			if (InsideFrustrum(App->scene->gameCameraComponent, componentMesh->mesh->GlobalAABB)) 
			{
				float4x4 matrix = float4x4::FromTRS(float3(5, 1, 1), Quat::identity, float3(1, 1, 1));

				glPushMatrix();
				glMultMatrixf(gameobject->transform->GetTransformMatrix().Transposed().ptr());
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_TEXTURE_COORD_ARRAY);
				//Bind Mesh
				glBindBuffer(GL_ARRAY_BUFFER, componentMesh->mesh->VBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, componentMesh->mesh->EBO);
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glVertexPointer(3, GL_FLOAT, sizeof(ModuleMesh::Vertex), (void*)0);

				//Bind Textures
				if (gameobject->GetComponent(ComponentTypes::TEXTURE) != nullptr)
				{
					const Texture* mTexture = dynamic_cast<ComponentTexture*>(gameobject->GetComponent(ComponentTypes::TEXTURE))->GetTexture();

					if (mTexture != nullptr)
					{


						glBindTexture(GL_TEXTURE_2D, mTexture->textID);
					}
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, checkTexture);
				}

				glNormalPointer(GL_FLOAT, sizeof(ModuleMesh::Vertex), (void*)offsetof(ModuleMesh::Vertex, Normal));
				glTexCoordPointer(2, GL_FLOAT, sizeof(ModuleMesh::Vertex), (void*)offsetof(ModuleMesh::Vertex, TexCoords));

				glDrawElements(GL_TRIANGLES, componentMesh->mesh->indices.size(), GL_UNSIGNED_INT, NULL);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				glDisable(GL_TEXTURE_2D);
				glDisableClientState(GL_VERTEX_ARRAY);
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_COORD_ARRAY);
				glPopMatrix();
				App->mesh->UpdateBoundingBoxes(gameobject->transform->GetTransformMatrix(), *componentMesh->mesh);
				App->mesh->RenderBoundingBoxes(*componentMesh->mesh);
			}
		}
	}
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	App->camera->EditorCamera->SetAspectRatio((float)width / (float)height);
	App->scene->gameCameraComponent->SetAspectRatio((float)width / (float)height);
}

const char* ModuleRenderer3D::GetFileExtension(const char* filePath) {
	const char* lastDot = strrchr(filePath, '.');
	if (lastDot != nullptr) {
		return lastDot + 1;
	}
	LOG("Invalid File Dropped");
	App->input->loadDirectory = false;
	return ""; // Return an empty string if no extension is found.
}


