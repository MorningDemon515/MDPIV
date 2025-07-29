
#include "Window.h"
#include "OGL/Renderer_GL.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"

#include <iostream>

using namespace MD_Math;

std::string title = "MDPIV";

unsigned int VBO, VAO, LightVAO;
Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f
};

int main()
{
    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    Input_Init(window.window);

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    shader.Link();
    
    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        (float)window.width / (float)window.height,
        0.1f,
        100.0f
    );

    float speed = 3.0f;
    MATRIX model = IdentityMatrix();
    
    static double lastTime = glfwGetTime();

    system("color a");
    Camera camera = Camera();
    while(window.Run())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        Input_Update(window.window);

        if(Input_IsKeyReleased(GLFW_KEY_ESCAPE))
            window.run = false;        

        camera.Move(speed * deltaTime, 50.0f * deltaTime);                        

        renderer->Clear(30, 30, 30);
        shader.Use();
        shader.SetMatrix("model", model);
        shader.SetMatrix("view", camera.Matrix());
        shader.SetMatrix("projection", projection);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &LightVAO);
    delete renderer;
    return 0;
}
