#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

class Window
{
public:
	Window() {};
	Window(int Width, int Height);
	~Window();

	void SetTitle(const char* title);
	void Quit(SDL_Event event);

	SDL_Window* window;
	bool running = true;
private:
	
};

#endif // !WINDOW_H
