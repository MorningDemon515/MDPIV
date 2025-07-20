#ifndef SHADER_H
#define SHADER_H

#include "MD_Math.h"

#ifdef __cplusplus
extern "C" {
#endif

void Shader_LoadFile(const char* VSFile, const char* FSFile);
void Shader_Set();
void Shader_Use();
void Shader_Clean();

void Shader_SetBool(const char* name, bool value);
void Shader_SetInt(const char* name, int value);
void Shader_SetFloat(const char* name, float value);

void Shader_SetVec2(const char* name,MD_MATH_VECTOR2 value);
void Shader_SetVec3(const char* name,MD_MATH_VECTOR3 value);
void Shader_SetVec4(const char* name,MD_MATH_VECTOR4 value);

void Shader_SetMatrix(const char* name, MD_MATH_MATRIX value);

#ifdef __cplusplus
}
#endif

#endif