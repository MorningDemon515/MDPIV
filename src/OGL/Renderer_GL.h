#ifndef RENDERER_GL_H
#define RENDERER_GL_H

#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>

#include "../Renderer.h"

class Renderer_GL : public Renderer
{
private:
    
public:
    ~Renderer_GL() override;
    void Init(GLFWwindow* window) override;
    void Clear(int red, int green, int blue) override;
    void Present(GLFWwindow* window) override;
};


#endif