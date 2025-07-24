#include "Main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3\SDL.h>
#include <SDL3\SDL_main.h>

#include <glad/glad.h>

#include "MD_Math.h"
#include "OpenGL3.3/Shader.h"

#include "Input.h"

using namespace MD_Math;

SDL_Window* Window;
SDL_Renderer* Renderer;
SDL_Event event;
bool run = true;

SDL_GLContext GLContext;

HWND WindowHandle = NULL;

float vertices[] = {
	 -0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,

	 -0.5f, -0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
	 -0.5f, 0.5f, 0.0f
};

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	Window = SDL_CreateWindow(
		"MDPIV",
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL
	);

	SDL_PropertiesID props = SDL_GetWindowProperties(Window);
    WindowHandle = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

	Renderer = SDL_CreateRenderer(
		Window,
		"opengl"
	);

	GLContext = SDL_GL_CreateContext(Window);
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	SetWindowTextA(WindowHandle, "MDPIV - OpenGL 3.3");

	SDL_Log("Available renderer drivers:");
	for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
		SDL_Log("%d. %s", i + 1, SDL_GetRenderDriver(i));
	}

	float x = 0.0f, y = 0.0f, speed = 5.0f;

	while (run)
	{
		Input_Update(&event);

		if (event.type == SDL_EVENT_QUIT)
			run = false;

		if (Input_IsKeyDown(event, SDLK_ESCAPE))
			run = false;

		//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
		//SDL_GL_SwapWindow(Window);

		speed = Input_GetMouseWheelDelta(event);
		SDL_Log("%f", speed);

		SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
		SDL_RenderClear(Renderer);

			y += speed;
			x += speed;
			
		SDL_FRect rect;
		rect.x = x;
		rect.y = y;
		rect.w = 100;
		rect.h = 100;

		SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(Renderer, &rect);
		

		SDL_RenderPresent(Renderer);

	}

	SDL_GL_DestroyContext(GLContext);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}

const char* ReadFile(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file) return NULL;

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char*)malloc(size + 1);
	if (!buffer) {
		fclose(file);
		return NULL;
	}

	size_t bytes_read = fread(buffer, 1, size, file);
	buffer[bytes_read] = '\0';

	fclose(file);
	return (const char*)buffer;
}
