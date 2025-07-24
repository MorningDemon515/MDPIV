
#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif
   
	void Input_Update(SDL_Event* event);

	int Input_IsKeyDown(SDL_Event event, int key);
	int Input_IsKeyPressed(SDL_Event event, int key);
	int Input_IsKeyReleased(SDL_Event event, int key);

	void Input_GetMousePosition(SDL_Event event, float* x, float* y);
	void Input_GetMouseDelta(SDL_Event event, float* x, float* y);
	int Input_IsMouseButtonDown(SDL_Event event, int mouse);
	int Input_IsMouseButtonPressed(SDL_Event event, int mouse);
	int Input_IsMouseButtonReleased(SDL_Event event, int mouse);
	float Input_GetMouseWheelDelta(SDL_Event event);

#ifdef __cplusplus
}
#endif

#endif