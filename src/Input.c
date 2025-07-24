#include "Input.h"

#ifdef __cplusplus
extern "C" {
#endif

	void Input_Update(SDL_Event* event)
	{
		SDL_PollEvent(event);
	}

	int Input_IsKeyDown(SDL_Event event, int key)
	{
		if (event.type == SDL_EVENT_KEY_DOWN)
			if (event.key.key == key)
				return 1;

		return 0;
	}

	int Input_IsKeyPressed(SDL_Event event, int key)
	{
		if (event.type == SDL_EVENT_KEY_DOWN)
			if (event.key.key == key)
				if (event.key.repeat == 0)
					return 1;

		return 0;
	}

	int Input_IsKeyReleased(SDL_Event event, int key)
	{
		if (event.type == SDL_EVENT_KEY_UP)
			if (event.key.key == key)
				return 1;

		return 0;
	}

	void Input_GetMousePosition(SDL_Event event, float* x, float* y)
	{
		if (event.type == SDL_EVENT_MOUSE_MOTION)
			*x = event.motion.x, *y = event.motion.y; 
		 
	}

	void Input_GetMouseDelta(SDL_Event event, float* x, float* y)
	{
		if (event.type == SDL_EVENT_MOUSE_MOTION)
			*x = event.motion.xrel, * y = event.motion.yrel;
	}

	int Input_IsMouseButtonDown(SDL_Event event, int mouse)
	{
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			if (event.button.down == mouse)
				return 1;

		static Uint32 last_state = 0;
		Uint32 current_state = SDL_GetMouseState(NULL, NULL);

		if (current_state != last_state) {
			last_state = current_state;
		}

		return (current_state & SDL_BUTTON_MASK(mouse)) != 0;
	}

	int Input_IsMouseButtonPressed(SDL_Event event, int mouse)
	{
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			if (event.button.button == mouse)
				return 1;

		return 0;
	}

	int Input_IsMouseButtonReleased(SDL_Event event, int mouse)
	{
		if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
			if (event.button.button == mouse)
				return 1;

		return 0;
	}

	float Input_GetMouseWheelDelta(SDL_Event event)
	{
		float delta = 0.0f;
		if (event.type == SDL_EVENT_MOUSE_WHEEL)
			delta += event.wheel.y;

		return delta;
	}

#ifdef __cplusplus
}
#endif