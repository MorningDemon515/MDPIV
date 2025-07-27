#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Renderer
{
private:
public:
    virtual ~Renderer(){};
    virtual void Init(GLFWwindow* window){};
    virtual void Clear(int red, int green, int blue){};
    virtual void Present(GLFWwindow* window){};
};

#endif