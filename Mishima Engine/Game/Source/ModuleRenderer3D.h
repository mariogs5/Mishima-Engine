#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "../Source/External/MathGeoLib/include/Math/float3x3.h"
#include "../Source/External/MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

#include "../Source/External/Glew/include/glew.h"
#include "../Source/External/SDL\include\SDL_opengl.h"
#include "../Source/External/ImGui/imgui.h"

// DevIL
#include "../Source/External/DevIL/include/ilut.h"
#include "../Source/External/DevIL/include/ilu.h"
#include "../Source/External/DevIL/include/il.h"

#include "ModuleMesh.h"
#include "ModuleTexture.h"

#include <string.h>

#define MAX_LIGHTS 8

//Checkers specs
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	void BindBuffers();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void InitDevil();

	//-------- Frustrum & Bounding Box --------//
	void DrawBox(float3* vertices, float3 color);
	bool InsideFrustrum(const ComponentCamera* camera, const AABB& aabb);

	//-------- GameObjects --------//
	void DrawGameObjects();

	void RenderFromCamera(ComponentCamera* camera, bool debug_draw_enabled);

	void DebugDraw(const Frustum& frustum, Color color, bool lines, const float& line_size);

	const char* GetFileExtension(const char* filePath);

	bool primCube = false;
	bool primSphere = false;
	bool primCylinder = false;
	bool primCone = false;
	bool primTorus = false;
	bool primBakerHouse = false;
	bool primAranara = false;
	bool primZhongli = false;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	Primitive* PrimitiveTest = nullptr;

	const char* myModel_path = nullptr;
	const char* myModel_texture_path = nullptr;

	uint VBO;
	uint EBO;
	uint VAO;

	bool activeWire = false;
	bool activeNormals = false;

	//Creat the scene on the window
	GLuint checkTexture;
	GLuint textureWidth, textureHeight;
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	std::vector<GameObject*> gameObject_list;
};