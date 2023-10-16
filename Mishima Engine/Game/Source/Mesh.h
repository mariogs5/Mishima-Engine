#pragma once

#include "../Source/External/Assimp/include/cimport.h"
#include "../Source/External/Assimp/include/scene.h"
#include "../Source/External/Assimp/include/postprocess.h"
//#pragma comment (lib, "..Source/External/Assimp/libx86/assimp.lib")

#include <iostream>
#include "vector"

#include "External/MathGeoLib/include/Math/float3.h"
#include "../Source/External/Glew/include/glew.h"

struct Vertex {
    float3 Position;
    float3 Normal;
    float3 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;  
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }
    //void Draw(Shader& shader);

private:

    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

