#include "Window.h"

#include "LoadImage.h"

Window::Window(int Width, int Height)
{
    glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, false);
    window = glfwCreateWindow(Width, Height, "", NULL, NULL);
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwSetWindowPos(window, mode->width /2 - Width / 2 , mode->height / 2 - Height / 2);

    width = Width; height = Height;
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::SetTitle(const char* title)
{
    glfwSetWindowTitle(window, title);
}

void Window::SetICON(const char* filename)
{
    Image ico;
    Load_Image_RGBA(filename, &ico);

    GLFWimage icon;
    icon.width = ico.width;
    icon.height = ico.height;
    icon.pixels = ico.pixels;
    glfwSetWindowIcon(window, 1, &icon);
}

int Window::Run()
{
    return !glfwWindowShouldClose(window) && run;
}

void Window::Quit()
{
    glfwPollEvents();      
}
