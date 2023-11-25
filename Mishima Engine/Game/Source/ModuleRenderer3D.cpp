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

	//Preguntar

	ilInit();

	App->mesh->LoadMesh("Assets/Primitives/BakerHouse.fbx");
	App->texture->LoadTexture("Assets/Primitives/Baker_House.png");
	BindBuffers();


	//CreateMainBuffer();



	// DevIL init
	//myTexture.DevILInit();

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Grid.axis = true;

	// Baker house 
	/*myModel.loadModel("Assets/Primitives/BakerHouse.fbx");
	myTexture.LoadTexture("Assets/Primitives/Baker_House.png");
	Models.push_back(myModel);*/

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	/*if (App->input->loadDirectory) {

		Model tempModel;
		tempModel.loadModel(App->input->dropped_filedir);
		Models.push_back(tempModel);
		App->input->loadDirectory = false;
	}*/

	// Drag and Drop Models & Textures
	//if (App->input->loadDirectory) {

	//	if (std::strcmp(GetFileExtension(App->input->dropped_filedir), "fbx") == 0) {

	//		Model tempModel;
	//		tempModel.loadModel(App->input->dropped_filedir);
	//		Models.push_back(tempModel);
	//		LOG("FBX load successful");
	//		App->input->loadDirectory = false;
	//	}
	//	if (std::strcmp(GetFileExtension(App->input->dropped_filedir), "FBX") == 0) {

	//		Model tempModel;
	//		tempModel.loadModel(App->input->dropped_filedir);
	//		Models.push_back(tempModel);
	//		LOG("FBX load successful");
	//		App->input->loadDirectory = false;
	//	}
	//	if (std::strcmp(GetFileExtension(App->input->dropped_filedir), "png") == 0) {

	//		Texture tempTexture;
	//		tempTexture.LoadTexture(App->input->dropped_filedir);
	//		//Textures.push_back(tempTexture);
	//		LOG("Texture load successful");
	//		App->input->loadDirectory = false;
	//	}
	//	else {

	//		App->input->loadDirectory = false;
	//	}
	//}

	// Primitive Cube
	//if (primCube) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/Cube.fbx");
	//	Models.push_back(tempModel);
	//	primCube = false;
	//}

	//// Primitive Sphere
	//if (primSphere) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/Sphere.fbx");
	//	Models.push_back(tempModel);
	//	primSphere = false;
	//}

	//// Primitive Cylinder
	//if (primCylinder) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/Cylinder.fbx");
	//	Models.push_back(tempModel);
	//	primCylinder = false;
	//}

	//// Primitive Cone
	//if (primCone) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/Cone.fbx");
	//	Models.push_back(tempModel);
	//	primCone = false;
	//}

	//// Primitive Torus
	//if (primTorus) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/Torus.fbx");
	//	Models.push_back(tempModel);
	//	primTorus = false;
	//}

	//// Primitive BakerHouse
	//if (primBakerHouse) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/BakerHouse.fbx");
	//	Models.push_back(tempModel);
	//	primBakerHouse = false;
	//}

	//// Primitive Aranara
	//if (primAranara) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/aranara.fbx");
	//	Models.push_back(tempModel);
	//	primAranara = false;
	//}

	//// Primitive Zhongli
	//if (primZhongli) {
	//	Model tempModel;
	//	tempModel.loadModel("Assets/Primitives/ze+hongli.fbx");
	//	Models.push_back(tempModel);
	//	primZhongli = false;
	//}

	Grid.Render();

	//myTexture.ActivateTexture();

	/*for (int i = 0; i < Models.size(); i++) 
	{
		Models[i].Draw();
	}

	myTexture.DeActivateTexture();*/
	//Draw Test
	/*glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glVertex3d(0, 1, 0);
	glEnd();*/

	//Mostrar una Esfera (sin acabar)
	/*CSphere s(1.0f, 20, 20);
	s.Render();*/

	App->editor->DrawEditor();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	//Models.clear();
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_GL_DeleteContext(context);

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

void ModuleRenderer3D::DrawBoundingBox(float3* vertices, float3 color)
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

//void ModuleRenderer3D::CreateMainBuffer()
//{
//	scene_render_texture = new RenderTexture();
//	scene_render_texture->Create(SCREEN_WIDTH, SCREEN_HEIGHT);
//
//	game_render_texture = new RenderTexture();
//	game_render_texture->Create(SCREEN_WIDTH, SCREEN_HEIGHT);
//}
//
//void ModuleRenderer3D::DeleteMainBuffer()
//{
//	delete(scene_render_texture);
//	delete(game_render_texture);
//}

//GLuint ModuleRenderer3D::GetSceneRenderTexture()
//{
//	return scene_render_texture->GetTexture();
//}
//
//GLuint ModuleRenderer3D::GetGameRenderTexture()
//{
//	return game_render_texture->GetTexture();
//}

void ModuleRenderer3D::DebugDrawBox(const float3* corners, Color color, bool lines, const float& line_size)
{
	GLint previous[2];
	if (lines)
	{
		glGetIntegerv(GL_POLYGON_MODE, previous);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glColor3f(color.r, color.g, color.b);

	glLineWidth(line_size);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[3]);

	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);
	glVertex3fv((GLfloat*)&corners[6]);

	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[7]);

	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[2]);

	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[2]);

	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[1]);

	glEnd();

	if (lines)
		glPolygonMode(GL_FRONT_AND_BACK, previous[0]);

	glLineWidth(1.0f);

	glColor3f(255, 255, 255);
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

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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


