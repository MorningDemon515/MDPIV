#include "Scene2.h"

#include "Texture.h"
#include "SimpleInput.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"

#include <glad/glad.h>

extern int run;
using namespace MD_Math;

VECTOR3 ComputerNormal(VECTOR3* p0,VECTOR3* p1, VECTOR3* p2 )
{
    VECTOR3 u(*p1 - *p0);
    VECTOR3 v(*p2 - *p0);

    return Vector3Normalized(VectorCross(u, v));
}

float points_2[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
};

VECTOR3 cube_Positions[] = {
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

Shader s_cube = Shader();
Shader l_cube = Shader();

Camera camera_2 = Camera();

VECTOR3 Light_Color(1.0f, 1.0f, 1.0f);
VECTOR3 LightPos(1.2f, 1.0f, 2.0f);

Scene2::Scene2()
{

}

Scene2::~Scene2()
{
   glDeleteBuffers(1,&VBO);
   glDeleteVertexArrays(1,&VAO);
  
   glDeleteVertexArrays(1,&LightVAO);
   glDeleteTextures(1, &texture);
}

MATRIX modellation
, modelangle_X
, modelangle_Y
, modelangle_Z
,model_;

MATRIX Light_Model = TranslationMatrix(LightPos.x, LightPos.y, LightPos.z) * 
                ScaleMatrix(0.5f, 0.5f, 0.5f);

MATRIX Projection = PerspectiveMatrixRH(
    AngularToRadian(45.0f),
    (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT,
    0.1f,
    100.0f
);

Material CubeMaterial = {
    {1.0f, 0.5f, 0.31f},
    {1.0f, 0.5f, 0.31f},
    {0.5f, 0.5f, 0.5f},
    32.0f
};

DirectionLight la = {
    {-0.2f, -1.0f, -0.3f},

    {0.2f, 0.2f, 0.2f},
    {0.5f, 0.5f, 0.5f},
    {1.0f, 1.0f, 1.0f}
};


void Scene2::InitEnv()
{
   
    /*
    for (int i = 0; i < 36; i += 3)
    {
         VECTOR3 temp = ComputerNormal(
            &points_vec3[i], 
            &points_vec3[i + 1], 
            &points_vec3[i + 2]
        );
        
        NormalVec3[i]     = temp;
        NormalVec3[i + 1] = temp;
        NormalVec3[i + 2] = temp;
    }*/
    
    /*
    float points_2[216]; 

    for (int i = 0, j = 0; i < 36; i++, j += 6)
    {
        points_2[j]     = points_vec3[i].x;
        points_2[j + 1] = points_vec3[i].y;
        points_2[j + 2] = points_vec3[i].z;
        
        points_2[j + 3] = NormalVec3[i].x;
        points_2[j + 4] = NormalVec3[i].y;
        points_2[j + 5] = NormalVec3[i].z;
    }*/
    
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points_2), points_2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    s_cube.LoadFile("resources/shader/cube_vs.txt", "resources/shader/cube_fs.txt");
    l_cube.LoadFile("resources/shader/cube_vs.txt", "resources/shader/cube_light_fs.txt");

    texture = T_LoadTextureRGB("resources/image.jpg");
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
    s_cube.SetMatrix("projection", Projection);
    s_cube.SetMatrix("view", camera_2.Matrix());
    s_cube.SetVec3("LightColor", Light_Color);
    s_cube.SetVec3("cameraPos",camera_2.Pos());
    s_cube.SetVec3("material.ambient",CubeMaterial.ambient);
    s_cube.SetVec3("material.diffuse",CubeMaterial.diffuse);
    s_cube.SetVec3("material.specular",CubeMaterial.specular);
    s_cube.SetFloat("material.shininess",CubeMaterial.shininess);
    s_cube.SetVec3("light.direction",  la.direction);
    s_cube.SetVec3("light.ambient",  la.ambient);
    s_cube.SetVec3("light.diffuse",  la.diffuse); 
    s_cube.SetVec3("light.specular", la.specular); 

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    for (unsigned int i = 0; i < 10; i++)
    {
         
        modellation = TranslationMatrix(cube_Positions[i].x , cube_Positions[i].y, cube_Positions[i].z);
        float angle = 20.0f * i;
        modelangle_X = RotationMatrix(AngularToRadian(angle), 'X');
        modelangle_Y = RotationMatrix(AngularToRadian(angle), 'Y');
        modelangle_Z = RotationMatrix(AngularToRadian(angle), 'Z');

        modelangle_Y *= 0.3f;
        modelangle_Z *= 0.5f;

        model_ = modellation * modelangle_X * modelangle_Y * modelangle_Z ;
        s_cube.SetMatrix("model", model_);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
    l_cube.Use();
    l_cube.SetMatrix("model", Light_Model);
    l_cube.SetMatrix("projection", Projection);
    l_cube.SetMatrix("view", camera_2.Matrix());
    l_cube.SetVec3("LightColor", Light_Color);

    glBindVertexArray(LightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}