#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>

class Renderer
{
public:
	virtual ~Renderer() = default;
	virtual void Init(SDL_Window* window) = 0;
	virtual void Clear(int Red, int Green, int Blue) = 0;
	virtual void Present(SDL_Window* window) = 0;
private:

};

#endif // !RENDERER_H
