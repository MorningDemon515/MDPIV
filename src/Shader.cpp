
#include "Shader.h"
#include <glad/glad.h>
#include <iostream>

Shader::Shader(const char* VSs, const char* FSs)
{
    VSS = ReadFile(VSs);
    FSS = ReadFile(FSs);
    GSS = nullptr;
}

Shader::Shader(const char* VSs, const char* FSs, const char* GSs)
{
    VSS = ReadFile(VSs);
    FSS = ReadFile(FSs);
    GSS = (GSs != nullptr) ? ReadFile(GSs) : nullptr;
}

Shader::~Shader()
{
    glDeleteProgram(Program);

    delete VSS;
    delete FSS;
    if (GSS != nullptr) {
        delete GSS;
    }
}

void Shader::Link()
{
    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VSS, NULL);
    glCompileShader(VertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FSS, NULL);
    glCompileShader(FragmentShader);
    
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    Program = glCreateProgram();
    glAttachShader(Program, VertexShader);
    
    unsigned int GeometryShader = 0;
    if (GSS != nullptr) {
        GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(GeometryShader, 1, &GSS, NULL);
        glCompileShader(GeometryShader);

        glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(GeometryShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        glAttachShader(Program, GeometryShader);
    }
    
    glAttachShader(Program, FragmentShader);
    glLinkProgram(Program);
    
    // 检查程序链接错误
    glGetProgramiv(Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    if (GSS != nullptr) {
        glDeleteShader(GeometryShader);
    }
}

void Shader::Use()
{
    glUseProgram(Program);
}

void Shader::SetInt(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(Program, name), value);
}

void Shader::SetFloat(const char* name, float value)
{
    glUniform1f(glGetUniformLocation(Program, name), value);
}

void Shader::SetVec2(const char* name, MD_Math::VECTOR2 value)
{
    glUniform2f(glGetUniformLocation(Program, name), value.x, value.y);
}

void Shader::SetVec3(const char* name, MD_Math::VECTOR3 value)
{
    glUniform3f(glGetUniformLocation(Program, name), value.x, value.y,value.z);
}

void Shader::SetVec4(const char* name, MD_Math::VECTOR4 value)
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

const char* Shader::ReadFile(const char* f)
{
    FILE* file = fopen(f, "r");  
    if (!file) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << f << std::endl;
        return NULL;
    }

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