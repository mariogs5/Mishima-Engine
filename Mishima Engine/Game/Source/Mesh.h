#pragma once

#include <iostream>
#include "vector"

#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"
#include "../Source/External/Glew/include/glew.h"

#include "../Source/External/DevIL/include/ilut.h"
#include "../Source/External/DevIL/include/ilu.h"
#include "../Source/External/DevIL/include/il.h"

#pragma comment (lib, "Game/Source/External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Game/Source/External/Devil/libx86/ILU.lib")
#pragma comment (lib, "Game/Source/External/Devil/libx86/ILUT.lib")

struct Vertex {
    float3 Position;
    float3 Normal;
    float2 TexCoords;
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

    //Texture import
    void DevILInit();
    void ImportTextures(const char* path);

    //void Draw(Shader& shader);
    void DrawMesh();

private:

    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

