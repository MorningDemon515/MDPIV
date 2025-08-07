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

#include "IBL.h"
#include "SkyBox.h"

using namespace MD_Math;

std::string title = "MDPIV";

unsigned int t1, t2, n;

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
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  
    //glEnable(GL_FRAMEBUFFER_SRGB);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Cube FinalSkyBox = Cube();
    Shader FinalSkyBoxShader = Shader("resources/glsl/Skybox_vs.txt","resources/glsl/Skybox_fs.txt");

    IBL ibl = IBL("resources/skybox/02.hdr");
    ibl.Set(FinalSkyBoxShader);

//////////////////////////////////////////////////////////////////////////////////////////////////////////    
    t1 = TextureFromFileRGB("resources/image.jpg");
    t2 = TextureFromFileRGBA("resources/image2.png");
    n = TextureFromFileRGB("resources/image_n.png");

    shader.Link();
    shader.Use();
    shader.SetInt("Texture", 0);
    shader.SetInt("texture_normal", 1);
    shader.SetInt("irradianceMap", 2);
    shader.SetInt("prefilterMap", 3);
    shader.SetInt("brdfLUT", 4);

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

    glViewport(0, 0, window.width, window.height);

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
        SetCubeTexture(ibl.irradianceMap, GL_TEXTURE2);
        SetCubeTexture(ibl.prefilterMap, GL_TEXTURE3);
        SetTexture(ibl.brdfLUTTexture, GL_TEXTURE4);

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
        ibl.Draw(FinalSkyBox, FinalSkyBoxShader);
        
        renderer->Present(window.window);
        
        window.Quit();
    }
    
    FreeTexture(t1);
    FreeTexture(t2);
    FreeTexture(n);
    delete renderer;
    return 0;
}