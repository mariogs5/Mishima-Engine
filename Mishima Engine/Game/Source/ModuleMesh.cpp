#include "ModuleMesh.h"
#include "Application.h"
#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"
#include <vector>
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "GameObject.h"

#pragma comment (lib, "Game/Source/External/Assimp/libx86/assimp.lib")

ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool ModuleMesh::Init()
{
	return true;
}

ModuleMesh::~ModuleMesh()
{}

update_status ModuleMesh::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleMesh::CleanUp()
{
	return true;
}

//-------- Bounding Boxes --------//
void ModuleMesh::InitBoundingBoxes() 
{
	for (int i = 0; i < ourMeshes.size(); i++) 
	{
		ourMeshes[i]->obb.SetNegativeInfinity();
		ourMeshes[i]->GlobalAABB.SetNegativeInfinity();

		std::vector<float3> floatArray;

		floatArray.reserve(ourMeshes[i]->ourVertex.size());

		for (const auto& vertex : ourMeshes[i]->ourVertex) {

			floatArray.push_back(vertex.Position);
		}
		ourMeshes[i]->aabb.SetFrom(&floatArray[0], floatArray.size());
	}
}

void ModuleMesh::UpdateBoundingBoxes(float4x4& transformMatrix, Mesh& moveMesh)
{
	moveMesh.obb = moveMesh.aabb;
	moveMesh.obb.Transform(transformMatrix);

	moveMesh.GlobalAABB.SetNegativeInfinity();
	moveMesh.GlobalAABB.Enclose(moveMesh.obb);
}

void ModuleMesh::RenderBoundingBoxes(Mesh& moveMesh)
{
	float3 verticesOBB[8];
	moveMesh.obb.GetCornerPoints(verticesOBB);
	externalapp->renderer3D->DrawBox(verticesOBB, float3(255, 0, 0));

	float3 verticesAABB[8];
	moveMesh.GlobalAABB.GetCornerPoints(verticesAABB);
	externalapp->renderer3D->DrawBox(verticesAABB, float3(0, 0, 255));
}

GameObject* ModuleMesh::LoadMesh(const char* file_path)
{
	const aiScene* imported_scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (imported_scene->HasMeshes() && imported_scene != nullptr)
	{
		GameObject* parentObject = App->scene->CreateGameObject("GameObject", App->scene->rootObject);

		GetSceneInfo(imported_scene->mRootNode, imported_scene, file_path, parentObject);

		aiReleaseImport(imported_scene);

		return parentObject;
	}

	LOG("Error loading scene % s", file_path);

	return nullptr;
}



void ModuleMesh::DrawNormals() {
	float length = 0.2f; // Define the length of the normals you want to draw

	glBegin(GL_LINES); // Start drawing lines

	for (Mesh* mesh : ourMeshes) {
		for (unsigned int i = 0; i < mesh->indices.size(); i += 3) {
			// Get the vertices of the triangle
			float3 vertex1 = mesh->ourVertex[mesh->indices[i]].Position;
			float3 vertex2 = mesh->ourVertex[mesh->indices[i + 1]].Position;
			float3 vertex3 = mesh->ourVertex[mesh->indices[i + 2]].Position;

			// Calculate the center of the triangle
			float3 center = (vertex1 + vertex2 + vertex3) / 3.0f;

			// Get the normal of the center (this assumes the normals are the same for all vertices in the triangle)
			float3 normal = mesh->ourVertex[mesh->indices[i]].Normal;

			// Calculate the end point of the normal
			float3 normalEnd = center + normal * length;

			// Set the color of the line (optional)
			glColor3f(1.0f, 1.0f, 0.0f); // Red color

			// Define the start and end points of the line
			glVertex3f(center.x, center.y, center.z);
			glVertex3f(normalEnd.x, normalEnd.y, normalEnd.z);
		}
	}

	glEnd(); // End drawing lines
}

void ModuleMesh::GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject)
{
	node->mTransformation.Decompose(scaling, rotation, translation);

	Scaling = { scaling.x, scaling.y, scaling.z };
	Rot = { rotation.x, rotation.y, rotation.z, rotation.w };
	Translation = { translation.x,translation.y,translation.z };

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		GetSceneInfo(node->mChildren[i], scene, file_path, gameObject);
	}

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		ProcessMesh(scene->mMeshes[node->mMeshes[i]], node, file_path, gameObject);
	}
}

ModuleMesh::Mesh ModuleMesh::ProcessMesh(aiMesh* mesh,aiNode* node, const char* file_path, GameObject* gameObject)
{
	Mesh* myMesh = new Mesh();

	GameObject* newMesh = App->scene->CreateGameObject(node->mName.C_Str(), gameObject);

	for (unsigned int j = 0; j < mesh->mNumVertices; j++)
	{
		Vertex vertex;
		float3 vector;
		vector.x = mesh->mVertices[j].x;
		vector.y = mesh->mVertices[j].y;
		vector.z = mesh->mVertices[j].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[j].x;
			vertex.Normal.y = mesh->mNormals[j].y;
			vertex.Normal.z = mesh->mNormals[j].z;

		}
		if (mesh->HasTextureCoords(0))
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][j].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][j].y;
		}
		else
		{
			vertex.TexCoords.x = 0.0f;
			vertex.TexCoords.y = 0.0f;
		}

		myMesh->ourVertex.push_back(vertex);
	}

	if (mesh->HasFaces())
	{
		myMesh->indices.resize(mesh->mNumFaces * 3);	//assume each face is a triangle

		for (uint y = 0; y < mesh->mNumFaces; y++)
		{
			if (mesh->mFaces[y].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&myMesh->indices[y * 3], mesh->mFaces[y].mIndices, 3 * sizeof(unsigned int));
			}
		}
	}

	ComponentMesh* mesh_component = (ComponentMesh*)newMesh->AddComponent(ComponentTypes::MESH);
	mesh_component->SetMesh(myMesh);
	mesh_component->SetPath(file_path);

	ourMeshes.push_back(myMesh);

	return *myMesh;
}
