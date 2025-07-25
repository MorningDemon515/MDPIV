#include "Window.h"

Window::Window(int width, int height)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		"",
		width, height,
		SDL_WINDOW_OPENGL
	);
}

Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void Window::Quit(SDL_Event event)
{
	if (event.type == SDL_EVENT_QUIT)
		running = false;
}