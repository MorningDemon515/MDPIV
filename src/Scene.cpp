#include "Scene.h"

#include "Texture.h"
#include "SimpleInput.h"
#include "Shader.h"

#include "Camera.h"

#include <glad/glad.h>

extern int run;
using namespace MD_Math;

MATRIX projection = PerspectiveMatrixRH(
    AngularToRadian(45.0f),
    (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT,
    0.1f,
    100.0f
);

VECTOR3 cameraPos = {0.0f, 0.0f,  3.0f};
VECTOR3 cameraFront = {0.0f, 0.0f, -1.0f};
VECTOR3 cameraUp = {0.0f, 1.0f,  0.0f};

MATRIX view = ViewMatrixRH(
        cameraPos,
        cameraPos + cameraFront,
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

VECTOR3 cubePositions[] = {
    VECTOR3( 0.0f,  0.0f,  0.0f),
    VECTOR3( 2.0f,  5.0f, -15.0f),
    VECTOR3(-1.5f, -2.2f, -2.5f),
    VECTOR3(-3.8f, -2.0f, -12.3f),
    VECTOR3( 2.4f, -0.4f, -3.5f),
    VECTOR3(-1.7f,  3.0f, -7.5f),
    VECTOR3( 1.3f, -2.0f, -2.5f),
    VECTOR3( 1.5f,  2.0f, -2.5f),
    VECTOR3( 1.5f,  0.2f, -1.5f),
    VECTOR3(-1.3f,  1.0f, -1.5f)
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
    
    Shader_SetMatrix("projection",projection);

}

MATRIX model = IdentityMatrix();
MATRIX model_lation = IdentityMatrix();
MATRIX model_angle_X = IdentityMatrix();
MATRIX model_angle_Y = IdentityMatrix();
MATRIX model_angle_Z = IdentityMatrix();

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

    for (unsigned int i = 0; i < 10; i++)
    {
         
        model_lation = TranslationMatrix(cubePositions[i].x , cubePositions[i].y, cubePositions[i].z);
        float angle = 20.0f * i;
        model_angle_X = RotationMatrix(AngularToRadian(angle), 'X');
        model_angle_Y = RotationMatrix(AngularToRadian(angle), 'Y');
        model_angle_Z = RotationMatrix(AngularToRadian(angle), 'Z');

        model_angle_Y *= 0.3f;
        model_angle_Z *= 0.5f;

        model = model_lation * model_angle_X * model_angle_Y * model_angle_Z ;
        Shader_SetMatrix("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}