#include "Input.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

static int currentKeys[GLFW_KEY_LAST + 1] = {0};
static int previousKeys[GLFW_KEY_LAST + 1] = {0};

void Input_Init(GLFWwindow* window)
{
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        currentKeys[i] = GLFW_RELEASE;
        previousKeys[i] = GLFW_RELEASE;
    }
}

void Input_Update(GLFWwindow* window)
{
    for (int i = 0; i <= GLFW_KEY_LAST; i++) {
        previousKeys[i] = currentKeys[i];
    }
    
    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++) {
        currentKeys[key] = glfwGetKey(window, key);
    }
}

int Input_IsKeyDown(int code)
{
    if (code < GLFW_KEY_SPACE || code > GLFW_KEY_LAST) 
        return 0;
    
    return currentKeys[code] == GLFW_PRESS;
}

int Input_IsKeyPressed(int code)
{
    if (code < GLFW_KEY_SPACE || code > GLFW_KEY_LAST) 
        return 0;
    
    return (currentKeys[code] == GLFW_PRESS) && 
           (previousKeys[code] == GLFW_RELEASE);
}

int Input_IsKeyReleased(int code)
{
    if (code < GLFW_KEY_SPACE || code > GLFW_KEY_LAST) 
        return 0;
    
    return (currentKeys[code] == GLFW_RELEASE) && 
           (previousKeys[code] == GLFW_PRESS);
} 

#ifdef __cplusplus
}
#endif