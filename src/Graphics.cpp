#include "Main.h"
#include "Graphics.h"

extern HWND WindowHanled;

Graphics::Graphics()
{
    hDC = GetDC(WindowHanled);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd  
        1,                                // version number  
        PFD_DRAW_TO_WINDOW |              // support window  
        PFD_SUPPORT_OPENGL |              // support OpenGL  
        PFD_DOUBLEBUFFER,                 // double buffered  
        PFD_TYPE_RGBA,                    // RGBA type  
        24,                               // 24-bit color depth  
        0, 0, 0, 0, 0, 0,                 // color bits ignored  
        0,                                // no alpha buffer  
        0,                                // shift bit ignored  
        0,                                // no accumulation buffer  
        0, 0, 0, 0,                       // accum bits ignored  
        32,                               // 32-bit z-buffer      
        0,                                // no stencil buffer  
        0,                                // no auxiliary buffer  
        PFD_MAIN_PLANE,                   // main layer  
        0,                                // reserved  
        0, 0, 0                           // layer masks ignored  
    };

    int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, iPixelFormat, &pfd);

}

Graphics::~Graphics()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hGL);
    ReleaseDC(WindowHanled, hDC);
}

void Graphics::Present()
{
    SwapBuffers(hDC);
    Sleep(1);
}

void Graphics::MakeContext()
{
    hGL = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGL);    
    
    gladLoadGL();

    SetWindowTextA(WindowHanled, "MDPIV - OpenGL 3.3");

    glEnable(GL_DEPTH_TEST);
}