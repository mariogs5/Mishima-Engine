#pragma once

#include "../Source/External/Assimp/include/cimport.h"
#include "../Source/External/Assimp/include/scene.h"
#include "../Source/External/Assimp/include/postprocess.h"
#include "../Source/External/Assimp/include/Importer.hpp"
#pragma comment (lib, "Game/Source/External/Assimp/libx86/assimp.lib")

#include "Mesh.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


//class Model
//{
//public:
//    Model(){}
//
//    Model(char* path)
//    {
//        loadModel(path);
//    }
//    void Draw();
//    void loadModel(std::string path);
//
//private:
//
//    // model data
//    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
//    std::vector<Mesh> meshes;
//    std::string directory;
//
//    void processNode(aiNode* node, const aiScene* scene);
//    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
//};
