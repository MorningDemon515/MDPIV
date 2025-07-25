#ifndef RENDERER_D3D9_H
#define RENDERER_D3D9_H

#include "../Renderer.h"

#include "../MD_Math.h"
#include <windows.h>
#include <d3d9.h>

class Renderer_D3D9 : public Renderer
{
public:
	~Renderer_D3D9() override;
	void Init(SDL_Window* window) override;
	void Clear(int Red, int Green, int Blue) override;
	void Present(SDL_Window* window) override;

private:
	IDirect3DDevice9* Device = nullptr;
	IDirect3D9* d3d9 = nullptr;
};

#endif // !RENDERER_D3D9_H
