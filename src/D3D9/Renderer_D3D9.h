#ifndef RENDERER_D3D9_H
#define RENDERER_D3D9_H

#include <windows.h>
#include <d3d9.h>

#include "../Renderer.h"

class Renderer_D3D9 : public Renderer
{
private:
    IDirect3D9* d3d9;
    IDirect3DDevice9* Device;
public:
    ~Renderer_D3D9() override;
    void Init(GLFWwindow* window) override;
    void Clear(int red, int green, int blue) override;
    void Present(GLFWwindow* window) override;

};

#endif