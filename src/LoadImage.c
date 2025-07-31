
#include "LoadImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

#ifdef __cplusplus
extern "C" {
#endif

void Load_Image_RGB(const char* filename, Image* image)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb);
    image->width = width;
    image->height = height;
    image->pixels = data;
}

void Load_Image_RGBA(const char* filename, Image* image)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    image->width = width;
    image->height = height;
    image->pixels = data;
}

void Load_Image_GL(const char* filename, Image* image)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
            
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
        //std::cout << "纹理加载成功: " << path << std::endl;
    } else {
        //std::cerr << "纹理加载失败: " << path << std::endl;
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    
    image->id = textureID;
}

#ifdef __cplusplus
}
#endif
