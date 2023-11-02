#pragma once

#include "../Source/External/DevIL/include/ilut.h"
#include "../Source/External/DevIL/include/ilu.h"
#include "../Source/External/DevIL/include/il.h"

#pragma comment (lib, "Game/Source/External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "Game/Source/External/Devil/libx86/ILU.lib")
#pragma comment (lib, "Game/Source/External/Devil/libx86/ILUT.lib")

#include <iostream>
#include "vector"

class Texture {
public:
    Texture(){}

    //Texture import
    void DevILInit();
    void LoadTexture(std::string path);
    void ActivateTexture();
    void DeActivateTexture();


private:
    unsigned int width;
    unsigned int height;
    std::string path;
    unsigned int textureID;
};
