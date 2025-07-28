
#include "Window.h"
#include "OGL/Renderer_GL.h"
#include "Shader.h"

#include <iostream>

using namespace MD_Math;

std::string title = "MDPIV";

unsigned int VBO, VAO;
Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");

float vertices[] = {
    -0.5, -0.5, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,

    -0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

int main()
{
    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    shader.Link();
    
    MATRIX view = ViewMatrixRH(
        VECTOR3(0.0f, 0.0f, 3.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 1.0f, 0.0f)
    );
    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        800.0f / 600.0f ,
        0.1f,
        100.0f
    );
    
    float x = 0.0f, y = 0.0f;
    MATRIX model = TranslationMatrix(x, y, 0.0f) * ScaleMatrix(0.5f, 0.5f, 0.5f);

    while(window.Run())
    {
        int state = glfwGetKey(window.window, GLFW_KEY_ESCAPE);
        if (state == GLFW_PRESS)
        {
            window.run = false;
        }
        
        renderer->Clear(30, 30, 30);
        
        shader.Use();
        shader.SetMatrix("model", model);
        shader.SetMatrix("view", view);
        shader.SetMatrix("projection", projection);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    delete renderer;
    return 0;
}