#include "Texture.h"
#include "Application.h"

void Texture::DevILInit()
{
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
    LOG("DevIl initialized");
}

void Texture::LoadTexture(std::string path)
{
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);
    if (ilLoadImage(path.c_str())) 
    {
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
        ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    }

    ilDeleteImages(1, &imageID);
}

void Texture::ActivateTexture() 
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::DeActivateTexture() 
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
