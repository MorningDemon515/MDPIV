#ifndef LOADIMAGE_H
#define LOADIMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Image
{
    unsigned int id;// for OpenGL

    int width;
    int height;
    unsigned char* pixels;
} Image;

void Load_Image_RGB(const char* filename, Image* image);
void Load_Image_RGBA(const char* filename, Image* image);

void Load_Image_GL(const char* filename, Image* image);

#ifdef __cplusplus
}
#endif

#endif