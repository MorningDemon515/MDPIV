#include "Renderer_GL.h"

#include <iostream>
#include <string>

extern std::string title;

Renderer_GL::~Renderer_GL()
{

}

void Renderer_GL::Init(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetWindowTitle(window, (title + " - OpenGL 3.3").c_str());
}

void Renderer_GL::Clear(int red, int green, int blue)
{
    glClearColor((float)(red) / 255.0f, (float)(green) / 255.0f, (float)(blue) / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer_GL::Present(GLFWwindow* window) 
{
    glfwSwapBuffers(window);
}