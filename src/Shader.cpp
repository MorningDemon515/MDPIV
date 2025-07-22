#include "Shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

const char* ReadFile(const char* filename) {
    FILE* file = fopen(filename, "r");  
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0'; 
    
    fclose(file);
    return (const char*)buffer;  
}

const char* VSS;
const char* FSS;
unsigned int Program;

void Shader_LoadFile(const char* VSFile, const char* FSFile)
{
    VSS = ReadFile(VSFile);
    FSS = ReadFile(FSFile);
}

void Shader_Clean()
{
    glDeleteProgram(Program);
    free((void*)VSS);
    free((void*)FSS);
}

void Shader_Set()
{
    unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1 , &VSS, NULL);
    glCompileShader(VertexShader);

    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FSS, NULL);
    glCompileShader(FragmentShader);

    Program = glCreateProgram();
    glAttachShader(Program, VertexShader);
    glAttachShader(Program, FragmentShader);
    glLinkProgram(Program);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void Shader_Use()
{
    glUseProgram(Program);
}

void Shader_SetBool(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(Program, name), (int)value);
}

void Shader_SetInt(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(Program, name), value);
}

void Shader_SetFloat(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(Program, name), value);
}

void Shader_SetVec2(const char* name,MD_Math::VECTOR2 value)
{
    glUniform2f(glGetUniformLocation(Program, name), value.x, value.y);
}

void Shader_SetVec3(const char* name,MD_Math::VECTOR3 value)
{
    glUniform3f(glGetUniformLocation(Program, name), value.x, value.y,value.z);
}

void Shader_SetVec4(const char* name,MD_Math::VECTOR4 value)
{
    glUniform4f(glGetUniformLocation(Program, name), value.x, value.y,value.z, value.w);
}

void Shader_SetMatrix(const char* name, MD_Math::MATRIX value)
{
    float Mat_ptr[16];
    MD_Math::MatrixToValue(value,Mat_ptr); 

    glUniformMatrix4fv(
        glGetUniformLocation(Program, name),
        1,
        GL_FALSE,
        Mat_ptr);
}