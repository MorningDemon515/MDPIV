#include "Scene2.h"

#include "Texture.h"
#include "SimpleInput.h"
#include "Shader.h"

#include "Camera.h"

#include <glad/glad.h>

extern int run;
using namespace MD_Math;

float points_2[] = {
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

Shader s_cube = Shader();
Shader l_cube = Shader();

Camera camera_2 = Camera();

VECTOR3 Light_Color(0.0f, 1.0f, 0.0f);
VECTOR3 Cube_Color(1.0f, 0.5f, 0.f);

Scene2::Scene2()
{

}

Scene2::~Scene2()
{
   glDeleteBuffers(1,&VBO);
   glDeleteVertexArrays(1,&VAO);
  
   glDeleteVertexArrays(1,&LightVAO);
}

MATRIX CUBE_Model = IdentityMatrix();
MATRIX Light_Model = TranslationMatrix(1.2f, 1.0f, 2.0f) * 
                ScaleMatrix(0.5f, 0.5f, 0.5f);

MATRIX Projection = PerspectiveMatrixRH(
    AngularToRadian(45.0f),
    (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT,
    0.1f,
    100.0f
);

void Scene2::InitEnv()
{
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points_2), points_2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    s_cube.LoadFile("resources/shader/cube_vs.txt", "resources/shader/cube_fs.txt");
    l_cube.LoadFile("resources/shader/cube_vs.txt", "resources/shader/cube_light_fs.txt");
}

void Scene2::Render()
{
    if(Input_IsKeyDown(VK_ESCAPE))
    {
        run = 0;
    }

    camera_2.Move();
    camera_2.Look();

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    s_cube.Use();
    s_cube.SetMatrix("model", CUBE_Model);
    s_cube.SetMatrix("projection", Projection);
    s_cube.SetMatrix("view", camera_2.Matrix());
    s_cube.SetVec3("LightColor", Light_Color);
    s_cube.SetVec3("CubeColor", Cube_Color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    l_cube.Use();
    l_cube.SetMatrix("model", Light_Model);
    l_cube.SetMatrix("projection", Projection);
    l_cube.SetMatrix("view", camera_2.Matrix());
    l_cube.SetVec3("LightColor", Light_Color);

    glBindVertexArray(LightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}