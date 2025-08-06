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
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_FRAMEBUFFER_SRGB);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Cube skybox = Cube();

    Shader skyboxshader("resources/glsl/ToCubemap_vs.txt","resources/glsl/ToCubemap_fs.txt");
    skyboxshader.Link();

    //MATRIX skyboxModel = ScaleMatrix(100.0f, 100.0f, 100.0f);
    
    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO); 

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf("resources/skybox/02.hdr", &width, &height, &nrComponents, 0);
    unsigned int hdrTexture;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                     512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    MATRIX captureProjection = PerspectiveMatrixRH(AngularToRadian(90.0f), 1.0f, 0.1f, 10.0f);
    MATRIX captureViews[] = 
    {
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 1.0f,  0.0f,  0.0f), VECTOR3(0.0f, -1.0f,  0.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(-1.0f,  0.0f,  0.0f), VECTOR3(0.0f, -1.0f,  0.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f,  1.0f,  0.0f), VECTOR3(0.0f,  0.0f,  1.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f, -1.0f,  0.0f), VECTOR3(0.0f,  0.0f, -1.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f,  0.0f,  1.0f), VECTOR3(0.0f, -1.0f,  0.0f)),
       ViewMatrixRH(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3( 0.0f,  0.0f, -1.0f), VECTOR3(0.0f, -1.0f,  0.0f))
    };

    // convert HDR equirectangular environment map to cubemap equivalent
    skyboxshader.Use();
    skyboxshader.SetInt("equirectangularMap", 0);
    skyboxshader.SetMatrix("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        skyboxshader.SetMatrix("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox.Draw(skyboxshader); // renders a 1x1 cube
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

    Cube FinalSkyBox = Cube();
    Shader FinalSkyBoxShader = Shader("resources/glsl/Skybox_vs.txt","resources/glsl/Skybox_fs.txt");
    FinalSkyBoxShader.Link();
    FinalSkyBoxShader.Use();
    FinalSkyBoxShader.SetInt("equirectangularMap", 0);

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

    float speed = 3.0f;
    MATRIX model = IdentityMatrix();
    MATRIX NM = NormalMatrix(model);

    MATRIX L_model = IdentityMatrix();

    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        (float)window.width / (float)window.height,
        0.1f,
        100.0f
    );
    
    static double lastTime = glfwGetTime();

    system("color a");
    Camera camera = Camera();

    PBR_Light pbr_light = {
        VECTOR3( 0.7f,  1.0f,  2.0f),
        VECTOR3(100.0f, 100.0f, 100.0f)
    };
    
    Mesh Q_Cube = Mesh(pos, texc, indices);
    Cube LCube = Cube();

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window.window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    while(window.Run())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        Input_Update(window.window);

        if(Input_IsKeyReleased(GLFW_KEY_ESCAPE))
            window.run = false;        

        camera.Move(speed * deltaTime, 50.0f * deltaTime);                

        //glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        renderer->Clear(0, 0, 0);
        //glEnable(GL_DEPTH_TEST);

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

        FinalSkyBoxShader.Use();
        FinalSkyBoxShader.SetMatrix("view", camera.Matrix());
        FinalSkyBoxShader.SetMatrix("projection", projection);
        
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        FinalSkyBox.Draw(FinalSkyBoxShader);
        
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