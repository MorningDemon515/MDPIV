#include "Renderer_D3D9.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <iostream>
#include <string>

extern std::string title;

Renderer_D3D9::~Renderer_D3D9()
{
    Device->Release();
    d3d9->Release();
}

void Renderer_D3D9::Init(GLFWwindow* window)
{
    int width, height;
    HWND hwnd;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    glfwGetWindowSize(window, &width, &height);
    hwnd = glfwGetWin32Window(window);

    D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

    d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,         
		hwnd,              
		D3DCREATE_HARDWARE_VERTEXPROCESSING,              
	    &d3dpp,             
	    &Device);      

    SetWindowTextA(hwnd, (title + " - Direct3D 9").c_str());    
}

void Renderer_D3D9::Clear(int red, int green, int blue)
{
    Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0);
}

void Renderer_D3D9::Present(GLFWwindow* window)
{
    Device->Present(0, 0, 0, 0);
}