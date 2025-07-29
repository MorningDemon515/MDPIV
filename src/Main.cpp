
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

unsigned int VBO, VAO, LightVAO, texture;
Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");
Shader L_shader = Shader("resources/glsl/light_vertex.txt", "resources/glsl/light_fragment.txt");

/*
Materials materials = {
    VECTOR3(1.0f, 0.5f, 0.31f),
    VECTOR3(1.0f, 0.5f, 0.31f),
    VECTOR3(0.5f, 0.5f, 0.5f),

    32.0f
};*/

Materials_Texture materials =
{
    VECTOR3(0.5f, 0.5f, 0.5f),

    32.0f
};

Light_Directional light = {
    //VECTOR3(1.2f, 1.0f, 2.0f),
    VECTOR3(-0.2f, -1.0f, -0.3f),

    VECTOR3(0.2f, 0.2f, 0.2f),
    VECTOR3(0.5f, 0.5f, 0.5f),
    VECTOR3(1.0f, 1.0f, 1.0f)
};

float vertices[] = {
     // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Image tex;
    Load_Image_RGB("resources/image.jpg", &tex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        tex.width, tex.height,
        0, GL_RGB,
        GL_UNSIGNED_BYTE,
        tex.pixels
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    shader.Link();
    L_shader.Link();
    
    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        (float)window.width / (float)window.height,
        0.1f,
        100.0f
    );

    float speed = 3.0f;
    MATRIX model = IdentityMatrix();
    MATRIX NM = NormalMatrix(model);

    //MATRIX L_model = TranslationMatrix(light.Position.x, light.Position.y, light.Position.z) * 
      //              ScaleMatrix(0.2f, 0.2f, 0.2f);
    
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
        shader.SetMatrix("view", camera.Matrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("nm", NM);

        shader.SetVec3("ViewPos", camera.Pos());

        //shader.SetVec3("materials.Ambinet", materials.Ambinet);
        //shader.SetVec3("materials.Diffuse", materials.Diffuse);
        shader.SetVec3("materials.Specular", materials.Specular);
        shader.SetFloat("materials.Power", materials.Power);

        shader.SetVec3("light.Direction", light.Direction);
        shader.SetVec3("light.Ambient", light.Ambient);
        shader.SetVec3("light.Diffuse", light.Diffuse);
        shader.SetVec3("light.Specular", light.Specular);

        shader.SetInt("Texture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
         
            model = TranslationMatrix(cube_Positions[i].x , cube_Positions[i].y, cube_Positions[i].z);
            shader.SetMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

/*
        L_shader.Use();
        L_shader.SetMatrix("model", L_model);
        L_shader.SetMatrix("view", camera.Matrix());
        L_shader.SetMatrix("projection", projection);
        
        glBindVertexArray(LightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
*/
        renderer->Present(window.window);
        
        window.Quit();
    }
    
    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &LightVAO);
    delete renderer;
    return 0;
}
