
#include "Window.h"
#include "OGL/Renderer_GL.h"

#include <iostream>

std::string title = "MDPIV";

int main()
{
    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    while(window.Run())
    {
        int state = glfwGetKey(window.window, GLFW_KEY_ESCAPE);
        if (state == GLFW_PRESS)
        {
            window.run = false;
        }
        
        renderer->Clear(30, 30, 30);
        
        renderer->Present(window.window);
        
        window.Quit();
    }
    
    delete renderer;
    return 0;
}