#include "Shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

const char* Shader::ReadFile(const char* filename) {
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

void Shader::SetBool(const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(Program, name), (int)value);
}

void Shader::SetInt(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(Program, name), value);
}

void Shader::SetFloat(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(Program, name), value);
}

void Shader::SetVec2(const char* name,MD_Math::VECTOR2 value)
{
    glUniform2f(glGetUniformLocation(Program, name), value.x, value.y);
}

void Shader::SetVec3(const char* name,MD_Math::VECTOR3 value)
{
    glUniform3f(glGetUniformLocation(Program, name), value.x, value.y,value.z);
}

void Shader::SetVec4(const char* name,MD_Math::VECTOR4 value)
{
    glUniform4f(glGetUniformLocation(Program, name), value.x, value.y,value.z, value.w);
}

void Shader::SetMatrix(const char* name, MD_Math::MATRIX value)
{
    float Mat_ptr[16];
    MD_Math::MatrixToValue(value,Mat_ptr); 

    glUniformMatrix4fv(
        glGetUniformLocation(Program, name),
        1,
        GL_FALSE,
        Mat_ptr);
}

Shader::Shader()
{
}

Shader::~Shader()
{
    glDeleteProgram(Program);
    free((void*)VSS);
    free((void*)FSS);
}

void Shader::LoadFile(const char* VSFile, const char* FSFile)
{
    VSS = ReadFile(VSFile);
    FSS = ReadFile(FSFile);

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

void Shader::Use()
{
    glUseProgram(Program);
}
