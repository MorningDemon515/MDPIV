#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

#ifdef __cplusplus
extern "C" {
#endif

void Input_Init(GLFWwindow* window);
void Input_Update(GLFWwindow* window);

int Input_IsKeyDown(int code);
int Input_IsKeyPressed(int code);
int Input_IsKeyReleased(int code); 

#ifdef __cplusplus
}
#endif

#endif