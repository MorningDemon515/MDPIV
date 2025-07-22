#ifndef SHADER_H
#define SHADER_H

#include "MD_Math.h"

/*
void Shader_LoadFile(const char* VSFile, const char* FSFile);
void Shader_Set();
void Shader_Use();
void Shader_Clean();

void Shader_SetBool(const char* name, bool value);
void Shader_SetInt(const char* name, int value);
void Shader_SetFloat(const char* name, float value);

void Shader_SetVec2(const char* name,MD_Math::VECTOR2 value);
void Shader_SetVec3(const char* name,MD_Math::VECTOR3 value);
void Shader_SetVec4(const char* name,MD_Math::VECTOR4 value);

void Shader_SetMatrix(const char* name, MD_Math::MATRIX value);
*/

class Shader
{
private:
   const char* VSS;
   const char* FSS;

   const char* ReadFile(const char* filename);
public:
    Shader();
    ~Shader();

    void LoadFile(const char* VSFile, const char* FSFile);
    void Use();

    void SetBool(const char* name, bool value);
    void SetInt(const char* name, int value);
    void SetFloat(const char* name, float value);

    void SetVec2(const char* name, MD_Math::VECTOR2 value);
    void SetVec3(const char* name, MD_Math::VECTOR3 value);
    void SetVec4(const char* name, MD_Math::VECTOR4 value);

    void SetMatrix(const char* name, MD_Math::MATRIX value);

    unsigned int Program;
};


#endif