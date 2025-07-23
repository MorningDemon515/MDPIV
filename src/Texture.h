#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned int T_LoadTextureRGB(const char* file);
unsigned int T_LoadTextureRGBA(const char* file);

#ifdef __cplusplus
}
#endif

#endif