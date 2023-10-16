#pragma once

#include "../Source/External/Assimp/include/cimport.h"
#include "../Source/External/Assimp/include/scene.h"
#include "../Source/External/Assimp/include/postprocess.h"
//#include "../Source/External/Assimp/include/Importer.hpp"

#include "Mesh.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
    Model(char* path)
    {
        loadModel(path);
    }
    //void Draw(Shader& shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    //Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
