#ifndef MODULE_MESH
#define MODULE_MESH

#include "Module.h"
#include "Globals.h"

#include "External/MathGeoLib/include/MathGeoLib.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/Quat.h"

#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"

#include "External/Glew/include/glew.h"

#include <vector>

#include "External/Glew/include/glew.h"

#include "External/MathGeoLib/include/Geometry/AABB.h"
#include "External/MathGeoLib/include/Geometry/OBB.h"

class GameObject;

class ModuleMesh : public Module
{
public:

	struct Vertex {
		float3 Position;
		float3 Normal;
		float2 TexCoords;
	};

	struct Mesh {
		std::vector<unsigned int> indices;
		std::vector<Vertex> ourVertex;
		unsigned int VBO = 0, EBO = 0;
		AABB GlobalAABB;
		AABB aabb;
		OBB obb;
	};

	ModuleMesh(Application* app, bool start_enabled = true);

	GameObject* LoadMesh(const char* file_path);

	~ModuleMesh();
	bool Init();
	update_status Update();
	bool CleanUp();
	void DrawNormals();

	//-------- Bounding Boxes --------//
	void InitBoundingBoxes();
	void UpdateBoundingBoxes(float4x4& transformMatrix, Mesh& moveMesh);
	void RenderBoundingBoxes(Mesh& moveMesh);

	void GetSceneInfo(aiNode* node, const aiScene* scene, const char* file_path, GameObject* gameObject);
	Mesh ProcessMesh(aiMesh* mesh, aiNode* node, const char* file_path, GameObject* gameObject);

	std::vector<Mesh*> ourMeshes;
	std::string name = "EmptyObject_";
	int num = 0;

	GameObject* newMesh;

	aiVector3D translation, scaling;
	aiQuaternion rotation;

	float3 Translation;
	float3 Scaling;
	Quat Rot;
};

#endif //MODULE_MESH

