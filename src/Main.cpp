
#include "Window.h"
#include "OGL/Renderer_GL.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Materials.h"
#include "Light.h"
#include "LoadImage.h"

#include <iostream>

using namespace MD_Math;

std::string title = "MDPIV";

int main()
{
    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    Input_Init(window.window);

    glEnable(GL_DEPTH_TEST);

    static double lastTime = glfwGetTime();

    system("color a");

    while(window.Run())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        Input_Update(window.window);

        if(Input_IsKeyReleased(GLFW_KEY_ESCAPE))
            window.run = false;        
   

        renderer->Clear(30, 30, 30);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    delete renderer;
    return 0;
}
