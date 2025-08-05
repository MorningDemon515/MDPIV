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
#include <vector>

#include "stb_image.h"

#include "Mesh.h"
#include "SkyBox.h"

using namespace MD_Math;

std::string title = "MDPIV";

unsigned int t1, t2, n;
unsigned int FBO, texture_fbo, RBO, quadVAO, quadVBO;

Shader shader = Shader("resources/glsl/PBR_vs.txt", "resources/glsl/PBR_fs.txt");
Shader L_shader = Shader("resources/glsl/light_vertex.txt", "resources/glsl/light_fragment.txt");

VECTOR3 pos[24] = {

    {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f},
   
    {-1.0f, -1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f},
   
    {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f},
    
    { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
  
    {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f, -1.0f},
   
    {-1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}
};

VECTOR2 texc[24] = {
   
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
  
    {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
 
    {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
  
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
    
    {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
 
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
};

std::vector<unsigned int> indices = {
  0,1,2,  0,2,3,    
    4,5,6,  4,6,7,    
    8,9,10, 8,10,11,  
    12,13,14,12,14,15,
    16,17,18,16,18,19,
    20,21,22,20,22,23 
};

int main()
{

/////////////////////////////////////////////////////////////////////////
    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    Input_Init(window.window);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_FRAMEBUFFER_SRGB);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    float quadVertices[] = { 
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    Shader FBshader("resources/glsl/FB_vs.txt", "resources/glsl/FB_fs.txt");
    FBshader.Link();

    FBshader.Use();
    FBshader.SetInt("screenTexture", 0);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &texture_fbo);
    glBindTexture(GL_TEXTURE_2D, texture_fbo);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.width, window.height, 0, GL_RGBA, GL_FLOAT, NULL);// HDR

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_fbo, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.width, window.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
    char faces[6][128] = 
{
    "resources/skybox/right.jpg",
    "resources/skybox/left.jpg",
    "resources/skybox/top.jpg",
    "resources/skybox/bottom.jpg",
    "resources/skybox/front.jpg",
    "resources/skybox/back.jpg"
};
 
    SkyBox SBox = SkyBox(faces);

    Shader skyboxshader("resources/glsl/Skybox_vs.txt","resources/glsl/Skybox_fs.txt");
    skyboxshader.Link();

    skyboxshader.Use();
    skyboxshader.SetInt("skybox", 0);

    MATRIX skyboxModel = ScaleMatrix(100.0f, 100.0f, 100.0f);
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////    
    t1 = TextureFromFileRGB("resources/image.jpg");
    t2 = TextureFromFileRGBA("resources/image2.png");
    n = TextureFromFileRGB("resources/image_n.png");

    shader.Link();
    shader.Use();
    shader.SetInt("Texture", 0);
    shader.SetInt("texture_normal", 1);
    shader.SetInt("irradianceMap", 2);

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

    PBR_Light pbr_light = {
        VECTOR3( 0.7f,  1.0f,  2.0f),
        VECTOR3(100.0f, 100.0f, 100.0f)
    };
    
    Mesh Q_Cube = Mesh(pos, texc, indices);
    Cube LCube = Cube();

    while(window.Run())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        Input_Update(window.window);

        if(Input_IsKeyReleased(GLFW_KEY_ESCAPE))
            window.run = false;        

        camera.Move(speed * deltaTime, 50.0f * deltaTime);                

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        renderer->Clear(0, 0, 0);
        glEnable(GL_DEPTH_TEST);

        shader.Use();
        shader.SetMatrix("view", camera.Matrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("nm", NM);

        shader.SetVec3("ViewPos", camera.Pos());

        shader.SetVec3("light.Position", pbr_light.Position);
        shader.SetVec3("light.Color", pbr_light.Color);

        SetTexture(t1, GL_TEXTURE0);
        SetTexture(n, GL_TEXTURE1);

        model = TranslationMatrix(0.0f , 0.0f, 0.0f) * ScaleMatrix(0.5f, 0.5f, 0.5f);
        shader.SetMatrix("model", model);

        Q_Cube.Draw(shader);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
        L_shader.Use();
        L_shader.SetMatrix("view", camera.Matrix());
        L_shader.SetMatrix("projection", projection);
                                
        L_model = TranslationMatrix(pbr_light.Position.x, pbr_light.Position.y, pbr_light.Position.z) * 
                ScaleMatrix(0.2f, 0.2f, 0.2f) * ScaleMatrix(0.5f, 0.5f, 0.5f);

        L_shader.SetMatrix("model", L_model);

        LCube.Draw(L_shader);

//////////////////////////////////////////////////////////////////////////////
/*
        skyboxshader.Use();
        skyboxshader.SetMatrix("model", skyboxModel);
        skyboxshader.SetMatrix("view", camera.Matrix());
        skyboxshader.SetMatrix("projection", projection);

        SBox.Draw(skyboxshader);
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, 0); 
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        FBshader.Use();  
        FBshader.SetFloat("exposure", 1.0f);
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, texture_fbo);
         glActiveTexture(GL_TEXTURE0); 
         glBindTexture(GL_TEXTURE_2D, texture_fbo);

        glDrawArrays(GL_TRIANGLES, 0, 6);  
        
        renderer->Present(window.window);
        
        window.Quit();
    }
    
    FreeTexture(t1);
    FreeTexture(t2);
    FreeTexture(n);
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &texture_fbo);
    glDeleteRenderbuffers(1, &RBO);
    delete renderer;
    return 0;
}