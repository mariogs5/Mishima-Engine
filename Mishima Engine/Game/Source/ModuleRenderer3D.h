#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "../Source/External/MathGeoLib/include/Math/float3x3.h"
#include "../Source/External/MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

#include "../Source/External/Glew/include/glew.h"
#include "../Source/External/SDL\include\SDL_opengl.h"

// DevIL
#include "../Source/External/DevIL/include/ilut.h"
#include "../Source/External/DevIL/include/ilu.h"
#include "../Source/External/DevIL/include/il.h"

#pragma comment (lib, "Game/Source/External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Game/Source/External/Devil/libx86/ILU.lib")
#pragma comment (lib, "Game/Source/External/Devil/libx86/ILUT.lib")

#include "Model.h"
#include "Texture.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	const char* GetFileExtension(const char* filePath);

	bool primCube = false;
	bool primSphere = false;
	bool primCylinder = false;
	bool primCone = false;
	bool primTorus = false;
	bool primBakerHouse = false;

	int isTexture = 0;
	int isFBX = 0;

public:

	Texture myTexture;
	Model myModel;
	std::vector<Texture> Textures;
	std::vector<Model> Models;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;
	
	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;
};