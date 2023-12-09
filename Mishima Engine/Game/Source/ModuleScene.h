#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include <vector>

class ModuleScene:public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init()override;
	update_status Update();
	bool CleanUp();

	GameObject* CreateGameObject(std::string name, GameObject* parent);

	GameObject* LoadMeshAndTexture(std::string path_mesh, std::string path_tex);

	GameObject* CreateChildObject(std::string name);
	std::vector<GameObject*> GetGameObjects();

	void UpdateGameObjects();

	std::vector<GameObject*> gameobjects;
	GameObject* rootObject = nullptr;

	GameObject* gameCamera;
	ComponentCamera* gameCameraComponent;
	std::vector<ComponentCamera*> cameras;
};
#endif // !MODULE_SCENE

