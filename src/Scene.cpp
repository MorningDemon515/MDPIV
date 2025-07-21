#include "Scene.h"

#include "Texture.h"
#include "SimpleInput.h"
#include "Shader.h"

#include "Camera.h"

#include <glad/glad.h>

extern int run;

MD_MATH_MATRIX model = MD_Math_IdentityMatrix;
MD_MATH_MATRIX projection = MD_Math_PerspectiveMatrixRH(
    MD_Math_AngularToRadian(45.0f),
    (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT,
    0.1f,
    100.0f
);

MD_MATH_VECTOR3 cameraPos = {0.0f, 0.0f,  3.0f};
MD_MATH_VECTOR3 cameraFront = {0.0f, 0.0f, -1.0f};
MD_MATH_VECTOR3 cameraUp = {0.0f, 1.0f,  0.0f};

MD_MATH_MATRIX view = MD_Math_ViewMatrixRH(
        cameraPos,
        MD_Math_Vector3Addition(cameraPos, cameraFront),
        cameraUp
    );

float points[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f 
};

Camera camera = Camera();

Scene::Scene()
{

}

Scene::~Scene()
{
   glDeleteBuffers(1,&VBO);
   glDeleteBuffers(1,&VAO);
   glDeleteTextures(1,&texture);

   Shader_Clean();

}

void Scene::InitEnv()
{
    glGenBuffers(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 *sizeof(float)));
    glEnableVertexAttribArray(1);

    T_SetTextureRGB("resources/image.jpg");
    
    Shader_LoadFile("resources/shader/texture_vs.txt",
                    "resources/shader/texture_fs.txt");
    Shader_Set();
    Shader_Use();
    
    Shader_SetMatrix("model",model); 
    Shader_SetMatrix("projection",projection);

}

void Scene::Render()
{
    if(Input_IsKeyDown(VK_ESCAPE))
    {
        run = 0;
    }
    
    camera.Move();
    camera.Look();

    Shader_SetMatrix("view", camera.Matrix());

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}