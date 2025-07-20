#ifndef SIMPLEINPUT_H
#define SIMPLEINPUT_H

#define MOUSE_LEFT    0x01
#define MOUSE_RIGHT   0x02
#define MOUSE_MIDDLE  0x04
#define MOUSE_X1      0x08
#define MOUSE_X2      0x10

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void Input_Initialize(HWND hwnd);

void Input_Update();

void Input_ProcessMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int Input_IsKeyDown(int vk_code);        
int Input_IsKeyPressed(int vk_code);      
int Input_IsKeyReleased(int vk_code);  

POINT Input_GetMousePosition();         
POINT Input_GetMouseDelta();           
int Input_IsMouseButtonDown(int button);  
int Input_IsMouseButtonPressed(int button);
int Input_IsMouseButtonReleased(int button); 
int Input_GetMouseWheelDelta();  

#ifdef __cplusplus
}
#endif

#endif