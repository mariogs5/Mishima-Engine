#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleMesh.h"
#include "ComponentMesh.h"
#include "ModuleTexture.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditor.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app,start_enabled)
{}

ModuleScene::~ModuleScene(){}

bool ModuleScene::Init()
{
	LOG("Loading base assets");

	rootObject = new GameObject("Scene");

	gameCamera = CreateGameObject("Game Camera", rootObject);

	gameobjects.push_back(rootObject);

	gameCameraComponent = new ComponentCamera(gameCamera);

	gameCameraComponent->SetPosition(float3(0.0f, 10.0f, 30.0f));
	gameCameraComponent->LookAt(float3(0.f, 0.f, 0.f));
	gameCameraComponent->SetAspectRatio((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);

	gameCamera->AddComponent(gameCameraComponent);

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

void ModuleScene::GameObjectPicking(const LineSegment& ray)
{
	std::map<float, ComponentMesh*> meshCandidates;
	
	for (auto it = App->renderer3D->gameObject_list.begin(); it != App->renderer3D->gameObject_list.end(); ++it) 
	{
		for (auto jt = (*it)->components.begin(); jt != (*it)->components.end(); ++jt)
		{
			if ((*jt)->type == MESH) 
			{
				ComponentMesh* meshToTest = (ComponentMesh*)(*jt);

				float closest;
				float furthest;

				if (ray.Intersects(meshToTest->mesh->GlobalAABB, closest, furthest)) {

					meshCandidates[closest] = meshToTest;
				}
			}
		}
	}

	std::vector<ComponentMesh*> meshesSorted;

	for (auto& candidate : meshCandidates) 
	{
		meshesSorted.push_back(candidate.second);
	}
	
	for (ComponentMesh* mesh : meshesSorted) 
	{
		if (mesh != nullptr && mesh->parent != nullptr) 
		{
			App->editor->GameObject_selected = nullptr;
		}
	}

	for (ComponentMesh* mesh : meshesSorted) {

		if (mesh != nullptr) {

			LineSegment localRay = ray;

			localRay.Transform(mesh->parent->transform->GetTransformMatrix());

			for (uint j = 0; j < mesh->mesh->indices.size(); j += 3) {

				uint triangle_indices[3] = { mesh->mesh->indices[j],mesh->mesh->indices[j + 1],mesh->mesh->indices[j + 2] };

				float3 point_a(mesh->mesh->ourVertex[triangle_indices[0]].Position);
				float3 point_b(mesh->mesh->ourVertex[triangle_indices[1]].Position);
				float3 point_c(mesh->mesh->ourVertex[triangle_indices[2]].Position);

				Triangle triangle(point_a, point_b, point_c);

				if (localRay.Intersects(triangle, nullptr, nullptr)) {

					if (mesh->parent != nullptr) 
					{
						App->editor->GameObject_selected = mesh->parent;

						for (auto it = App->renderer3D->gameObject_list.begin(); it != App->renderer3D->gameObject_list.end(); ++it) 
						{
							if ((*it) != mesh->parent) {

								(*it)->selected = false;
							}
						}
					}
					return;
				}
			}
		}
	}

	for (auto it = App->renderer3D->gameObject_list.begin(); it != App->renderer3D->gameObject_list.end(); ++it) 
	{
		(*it)->selected = false;
		App->editor->GameObject_selected = nullptr;
	}
}

