#pragma once

#include <iostream>
#include "vector"

#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float2.h"
#include "../Source/External/Glew/include/glew.h"

#include "Texture.h"

struct Vertex {
    float3 Position;
    float3 Normal;
    float2 TexCoords;
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
    void DrawMesh();

    //  render data
    unsigned int VAO, VBO, EBO;

private:

    void setupMesh();
};

