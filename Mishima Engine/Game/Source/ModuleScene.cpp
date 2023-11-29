#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleMesh.h"
#include "ModuleTexture.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app,start_enabled)
{
}

ModuleScene::~ModuleScene(){}

bool ModuleScene::Init()
{
	LOG("Loading base assets");

	rootObject = new GameObject("Scene");
	gameobjects.push_back(rootObject);

	return true;
}

update_status ModuleScene::Update()
{
	UpdateGameObjects();
	return update_status();
}

bool ModuleScene::CleanUp()
{
	LOG("Shut down scene");

	return true;
}

GameObject* ModuleScene::CreateGameObject(std::string name, GameObject* parent)
{
	GameObject* newObject = new GameObject(name);

	if (parent == nullptr)
	{
		newObject->SetParent(rootObject);

	}
	else
	{
		parent->AddChildren(newObject);
	}
	gameobjects.push_back(newObject);
	return newObject;
}

GameObject* ModuleScene::LoadMeshAndTexture(std::string path_mesh, std::string path_tex)
{
	GameObject* gameObject = App->mesh->LoadMesh(path_mesh.c_str());
	for (std::vector<GameObject*>::iterator it = gameObject->Children.begin(); it != gameObject->Children.end(); it++)
	{
		App->texture->LoadTextureToGameObject((*it), path_tex);
	}

	return gameObject;
}

GameObject* ModuleScene::CreateChildObject(std::string name)
{
	GameObject* newObject = new GameObject(name);
	rootObject->AddChildren(newObject);
	newObject->SetParent(rootObject);
	return newObject;
}

std::vector<GameObject*> ModuleScene::GetGameObjects()
{
	return gameobjects;
}

void ModuleScene::UpdateGameObjects()
{
	for (std::vector<GameObject*>::iterator it = gameobjects.begin(); it != gameobjects.end(); it++)
	{
		GameObject* update = *it;
		update->Update();
	}
}

