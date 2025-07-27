#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window
{
private:
    
public:
    GLFWwindow* window;
    bool run = true;

    Window(){};
    Window(int Width, int Height);
    ~Window();

    void SetTitle(const char* title);
    void SetICON(const char* filename);
    int Run();
    void Quit();
    
};

#endif