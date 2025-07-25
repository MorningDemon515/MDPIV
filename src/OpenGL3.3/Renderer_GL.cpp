#include "Renderer_GL.h"

#include <glad/glad.h>
#include "../Main.h"

Renderer_GL::~Renderer_GL()
{
	SDL_GL_DestroyContext(GLContext);
}

void Renderer_GL::Init(SDL_Window* window)
{
	GLContext = SDL_GL_CreateContext(window);
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	SDL_SetWindowTitle(window, (Window_Title + " - OpenGL 3.3").c_str());
}

void Renderer_GL::Clear(int Red, int Green, int Blue) 
{
	glClearColor((float)(Red / 255), (float)(Green / 255), (float)(Blue / 255), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer_GL::Present(SDL_Window* window)
{
	SDL_GL_SwapWindow(window);
}