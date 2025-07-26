#include <GLFW/glfw3.h>

int main()
{
    GLFWwindow* window;

    glfwInit();

    window = glfwCreateWindow(800, 600, "MDPIV", NULL, NULL);

    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}