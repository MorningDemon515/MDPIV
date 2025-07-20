#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include <glad/glad.h>

class Graphics
{
private:
    HDC hDC = nullptr;
    HGLRC hGL = nullptr;
public:
    Graphics();
    ~Graphics();
    
    void MakeContext();
    void Present();

};


#endif