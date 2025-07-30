
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

unsigned int VBO, VAO, EBO; 

float vertices[] = {
    0.5f, 0.5f, 0.0f,   
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f  
};

unsigned int indices[] = {

    0, 1, 3, 
    1, 2, 3  
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

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");
    shader.Link();

    MATRIX model = IdentityMatrix();
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
        shader.Use();
        shader.SetMatrix("model", model);
        shader.SetMatrix("view", view);
        shader.SetMatrix("projection", projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete renderer;
    return 0;
}
