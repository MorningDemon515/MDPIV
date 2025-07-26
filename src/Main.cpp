
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "LoadImage.h"

int main()
{
    GLFWwindow* window;

    glfwInit();
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(800, 600, "MDPIV", NULL, NULL);

    Image ico;
    Load_Image_RGBA("resources/icon/MorningDemon.jpg", &ico);

    GLFWimage icon;
    icon.width = ico.width;
    icon.height = ico.height;
    icon.pixels = ico.pixels;
    glfwSetWindowIcon(window, 1, &icon);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}