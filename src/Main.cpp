
#include "Window.h"
#include "OGL/Renderer_GL.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Materials.h"
#include "Light.h"
#include "LoadImage.h"
#include "Texture.h"

#include <iostream>

using namespace MD_Math;

std::string title = "MDPIV";

unsigned int VBO, VAO, LightVAO, t1, t2;
Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");
Shader L_shader = Shader("resources/glsl/light_vertex.txt", "resources/glsl/light_fragment.txt");

Materials_Texture materials =
{
    32.0f
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

VECTOR3 pointLightPositions[] = {
    VECTOR3( 0.7f,  0.2f,  2.0f),
    VECTOR3( 2.3f, -3.3f, -4.0f),
    VECTOR3(-4.0f,  2.0f, -12.0f),
    VECTOR3( 0.0f,  0.0f, -3.0f)
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

    t1 = TextureFromFile("resources/image.jpg");
    t2 = TextureFromFile("resources/image2.png");

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

    MATRIX L_model = IdentityMatrix();
    
    static double lastTime = glfwGetTime();

    system("color a");
    Camera camera = Camera();

    Light_Directional light_dir = {
        VECTOR3(-0.2f, -1.0f, -0.3f),
        
        VECTOR3(0.05f, 0.05f, 0.05f),
        VECTOR3(0.4f, 0.4f, 0.4f),
        VECTOR3(0.5f, 0.5f, 0.5f)
    };

    Light_Point light_point0 = {
        pointLightPositions[0],

        VECTOR3(0.05f, 0.05f, 0.05f),
        VECTOR3(0.8f, 0.8f, 0.8f),
        VECTOR3(1.0f, 1.0f, 1.0f),

        1.0f, 0.09f, 0.032f
    };

    Light_Point light_point1 = {
        pointLightPositions[1],

        VECTOR3(0.05f, 0.05f, 0.05f),
        VECTOR3(0.8f, 0.8f, 0.8f),
        VECTOR3(1.0f, 1.0f, 1.0f),

        1.0f, 0.09f, 0.032f
    };

    Light_Point light_point2 = {
        pointLightPositions[2],

        VECTOR3(0.05f, 0.05f, 0.05f),
        VECTOR3(0.8f, 0.8f, 0.8f),
        VECTOR3(1.0f, 1.0f, 1.0f),

        1.0f, 0.09f, 0.032f
    };

    Light_Point light_point3 = {
       pointLightPositions[3],

        VECTOR3(0.05f, 0.05f, 0.05f),
        VECTOR3(0.8f, 0.8f, 0.8f),
        VECTOR3(1.0f, 1.0f, 1.0f),

        1.0f, 0.09f, 0.032f
    };

    Light_Spot light_spot = {
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        0.0f, 0.0f,
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        0.0f, 0.0f,0.0f
    };
    
    light_spot.cutOff = Cos(AngularToRadian(12.5f));
    light_spot.outerCutOff = Cos(AngularToRadian(17.5f));

    light_spot.Ambient = VECTOR3(0.05f, 0.05f, 0.05f);
    light_spot.Diffuse = VECTOR3(0.8f, 0.8f, 0.8f);
    light_spot.Specular = VECTOR3(1.0f, 1.0f, 1.0f);

    light_spot.Constant = 1.0f;
    light_spot.Linear = 0.09f;
    light_spot.Quadratic = 0.032f;

    while(window.Run())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        Input_Update(window.window);

        if(Input_IsKeyReleased(GLFW_KEY_ESCAPE))
            window.run = false;        

        camera.Move(speed * deltaTime, 50.0f * deltaTime);    

        light_spot.Position = camera.Pos();
        light_spot.Direction = camera.Front();               

        renderer->Clear(0, 0, 0);
        shader.Use();
        shader.SetMatrix("view", camera.Matrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("nm", NM);

        shader.SetVec3("ViewPos", camera.Pos());

        //shader.SetVec3("materials.Ambinet", materials.Ambinet);
        //shader.SetVec3("materials.Diffuse", materials.Diffuse);
        shader.SetFloat("materials.Power", materials.Power);

        shader.SetVec3("light_dir.Direction", light_dir.Direction);
        shader.SetVec3("light_dir.Ambient", light_dir.Ambient);
        shader.SetVec3("light_dir.Diffuse", light_dir.Diffuse);
        shader.SetVec3("light_dir.Specular", light_dir.Specular);

        shader.SetVec3("light_point[0].Position", light_point0.Position);
        shader.SetVec3("light_point[0].Ambient", light_point0.Ambient);
        shader.SetVec3("light_point[0].Diffuse", light_point0.Diffuse);
        shader.SetVec3("light_point[0].Specular", light_point0.Specular);
        shader.SetFloat("light_point[0].Constant", light_point0.Constant);
        shader.SetFloat("light_point[0].Linear", light_point0.Linear);
        shader.SetFloat("light_point[0].Quadratic", light_point0.Quadratic);

        shader.SetVec3("light_point[1].Position", light_point1.Position);
        shader.SetVec3("light_point[1].Ambient", light_point1.Ambient);
        shader.SetVec3("light_point[1].Diffuse", light_point1.Diffuse);
        shader.SetVec3("light_point[1].Specular", light_point1.Specular);
        shader.SetFloat("light_point[1].Constant", light_point1.Constant);
        shader.SetFloat("light_point[1].Linear", light_point1.Linear);
        shader.SetFloat("light_point[1].Quadratic", light_point1.Quadratic);

        shader.SetVec3("light_point[2].Position", light_point2.Position);
        shader.SetVec3("light_point[2].Ambient", light_point2.Ambient);
        shader.SetVec3("light_point[2].Diffuse", light_point2.Diffuse);
        shader.SetVec3("light_point[2].Specular", light_point2.Specular);
        shader.SetFloat("light_point[2].Constant", light_point2.Constant);
        shader.SetFloat("light_point[2].Linear", light_point2.Linear);
        shader.SetFloat("light_point[2].Quadratic", light_point2.Quadratic);

        shader.SetVec3("light_point[3].Position", light_point3.Position);
        shader.SetVec3("light_point[3].Ambient", light_point3.Ambient);
        shader.SetVec3("light_point[3].Diffuse", light_point3.Diffuse);
        shader.SetVec3("light_point[3].Specular", light_point3.Specular);
        shader.SetFloat("light_point[3].Constant", light_point3.Constant);
        shader.SetFloat("light_point[3].Linear", light_point3.Linear);
        shader.SetFloat("light_point[3].Quadratic", light_point3.Quadratic);

        shader.SetVec3("light_spot.Position", light_spot.Position);
        shader.SetVec3("light_spot.Direction", light_spot.Direction);
        shader.SetVec3("light_spot.Ambient", light_spot.Ambient);
        shader.SetVec3("light_spot.Diffuse", light_spot.Diffuse);
        shader.SetVec3("light_spot.Specular", light_spot.Specular);
        shader.SetFloat("light_spot.cutOff", light_spot.cutOff);
        shader.SetFloat("light_spot.outerCutOff", light_spot.outerCutOff);
        shader.SetFloat("light_spot.Constant", light_spot.Constant);
        shader.SetFloat("light_spot.Linear", light_spot.Linear);
        shader.SetFloat("light_spot.Quadratic", light_spot.Quadratic);

        shader.SetInt("texture_diffuse", 0);
        SetTexture(t1, GL_TEXTURE0);

        shader.SetInt("texture_specular", 1);
        SetTexture(t2, GL_TEXTURE1);

        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
         
            model = TranslationMatrix(cube_Positions[i].x , cube_Positions[i].y, cube_Positions[i].z);
            shader.SetMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        L_shader.Use();
        L_shader.SetMatrix("view", camera.Matrix());
        L_shader.SetMatrix("projection", projection);
        
        glBindVertexArray(LightVAO);
        for(unsigned int j = 0; j < 4; j++)
        {
            L_model = TranslationMatrix(pointLightPositions[j].x, pointLightPositions[j].y, pointLightPositions[j].z) * 
                     ScaleMatrix(0.2f, 0.2f, 0.2f);

            L_shader.SetMatrix("model", L_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        renderer->Present(window.window);
        
        window.Quit();
    }
    
    FreeTexture(t1);
    FreeTexture(t2);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &LightVAO);
    delete renderer;
    return 0;
}
