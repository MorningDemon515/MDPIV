#include "Main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3\SDL.h>
#include <SDL3\SDL_main.h>

#include "MD_Math.h"
#include "Input.h"
#include "Window.h"
#include "OpenGL3.3/Shader.h"
#include "OpenGL3.3/Renderer_GL.h"
#include "Renderer.h"

using namespace MD_Math;

SDL_Event event;
bool run = true;

int main(int argc, char* argv[])
{

	Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT);
	window.SetTitle(Window_Title.c_str());

	Renderer* renderer = new Renderer_GL();
	renderer->Init(window.window);

	while (window.running)
	{
		Input_Update(&event);
		window.Quit(event);

		renderer->Clear(0, 255, 0);

		renderer->Present(window.window);
	}

	delete renderer;
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

float DeltaTime()
{
	static float lastTime = (float)SDL_GetTicks();
	float currTime = (float)SDL_GetTicks();
	float timeDelta = (currTime - lastTime) * 0.001f;

	lastTime = currTime;
	return timeDelta;
}
