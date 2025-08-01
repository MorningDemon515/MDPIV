
#include "Window.h"
#include "OGL/Renderer_GL.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Materials.h"
#include "Light.h"
#include "LoadImage.h"

#include <iostream>
#include "stb_image.h"

#include "Model.h"

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
    
    //stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    MATRIX model = IdentityMatrix() * ScaleMatrix(0.1f, 0.1f, 0.1f);
    MATRIX view = ViewMatrixRH(
        VECTOR3(0.0f, 0.0f, 3.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 1.0f, 0.0)
    );
    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        (float)window.width / (float)window.height,
        0.1f,
        100.0f
    );
    MATRIX NM = NormalMatrix(model);
    
    Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");
    shader.Link();
     
    Model scp173 = Model("resources/model/scp-173.obj");

    Camera camera = Camera();

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

        camera.Move(2.0f * deltaTime, 50.0f * deltaTime);

        renderer->Clear(30, 30, 30);

        shader.Use();
        shader.SetMatrix("model", model);
        shader.SetMatrix("view", camera.Matrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("nm", NM);

        scp173.Draw(shader);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    delete renderer;
    return 0;
}