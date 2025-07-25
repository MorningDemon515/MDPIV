#ifndef RENDERER_GL_H
#define RENDERER_GL_H

#include "../Renderer.h"
#include <SDL3/SDL.h>

class Renderer_GL : public Renderer
{
public:

	~Renderer_GL() override;
	void Init(SDL_Window* window) override;
	void Clear(int Red, int Green, int Blue) override;
    void Present(SDL_Window* window) override;

private:
	SDL_GLContext GLContext;
};

#endif // !RENDERER_GL_H
