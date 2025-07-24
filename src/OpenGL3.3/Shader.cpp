#include "Shader.h"
#include "../Main.h"

#include <glad/glad.h>

Shader::Shader(const char* VSF, const char* FSF)
{
	VertexShaderSource = ReadFile(VSF);
	FragmentShaderSource = ReadFile(FSF);

}

Shader::~Shader()
{
	free((void*)VertexShaderSource);
	free((void*)FragmentShaderSource);

	glDeleteProgram(Program);
}

void Shader::Use()
{
	glUseProgram(Program);
}

void Shader::Link()
{
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(VertexShader);

	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);

	Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
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
	glUniform3f(glGetUniformLocation(Program, name), value.x, value.y, value.z);
}

void Shader::SetVec4(const char* name, MD_Math::VECTOR4 value)
{
	glUniform4f(glGetUniformLocation(Program, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix(const char* name, MD_Math::MATRIX value)
{
	float value_ptr[16];
	MD_Math::MatrixToValue(value, value_ptr);
	glUniformMatrix4fv(glGetUniformLocation(Program, name), 1, GL_FALSE,value_ptr);
}

