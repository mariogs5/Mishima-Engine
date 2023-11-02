#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app,start_enabled)
{
}

ModuleScene::~ModuleScene(){}

bool ModuleScene::Init()
{
	LOG("Loading base assets");
	bool ret = true;

	return ret;
}

update_status ModuleScene::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	LOG("Shut down scene");

	return true;
}

GameObject* ModuleScene::CreateGameObject(std::string name)
{
	GameObject* newGameObject = new GameObject(name);
	gameobjects.push_back(newGameObject);
	newGameObject->SetParent(rootObject);
	return newGameObject;
}

std::vector<GameObject*> ModuleScene::GameObjects()
{
	return gameobjects;
}

