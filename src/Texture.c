#include "Texture.h"

#include "LoadImage.h"

#include <glad/glad.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int TextureFromFile(const char* filename)
{
    unsigned int texture;

    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Image tex;
    Load_Image_RGB(filename, &tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        tex.width, tex.height,
        0, GL_RGB,
        GL_UNSIGNED_BYTE,
        tex.pixels
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

void SetTexture(unsigned int id ,unsigned int level)
{
    glActiveTexture(level);
    glBindTexture(GL_TEXTURE_2D, id);
}

void FreeTexture(unsigned int id)
{
    glDeleteTextures(1, &id);
}

#ifdef __cplusplus
}
#endif