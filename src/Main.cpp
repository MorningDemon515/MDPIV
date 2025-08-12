
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
#include "Animator.h"

#include "Font.h"

using namespace MD_Math;

std::string title = "MDPIV";

Shader shader = Shader("resources/glsl/PBR_vs.txt", "resources/glsl/PBR_fs.txt");
Shader L_shader = Shader("resources/glsl/light_vertex.txt", "resources/glsl/light_fragment.txt");

std::vector<VECTOR3> pos = {

    {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f},
   
    {-1.0f, -1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f},
   
    {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f},
    
    { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
  
    {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f, -1.0f},
   
    {-1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}
};

std::vector<VECTOR2> texc = {
   
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
    PBR_Light pbr_light = {
        VECTOR3( 0.7f,  1.0f,  2.0f),
        VECTOR3(100.0f, 100.0f, 100.0f)
    };

    Model scp173 = Model("resources/model/scp173.fbx");
    Animator scp173Anim = Animator(&scp173);

    unsigned int normal = TextureFromFile("resources/model/173_Norm.jpg");

    shader.Link();
    shader.Use();

    shader.SetInt("irradianceMap", scp173.texCount);
    shader.SetInt("prefilterMap", scp173.texCount + 1);
    shader.SetInt("brdfLUT", scp173.texCount + 2);
    shader.SetInt("texture_normal0", scp173.texCount + 3);

    L_shader.Link();

    float speed = 3.0f;
    MATRIX model = ScaleMatrix(0.001f, 0.001f, 0.001f);
    MATRIX NM = NormalMatrix(model);

    MATRIX L_model = TranslationMatrix(pbr_light.Position.x, pbr_light.Position.y, pbr_light.Position.z) * 
                ScaleMatrix(0.2f, 0.2f, 0.2f) * ScaleMatrix(0.5f, 0.5f, 0.5f);

    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        (float)window.width / (float)window.height,
        0.1f,
        100.0f
    );

    static double lastTime = glfwGetTime();

    system("color a");
    Camera camera = Camera();
    
    Cube LCube = Cube();

    glViewport(0, 0, window.width, window.height);
    
    int mode = 1;
    
    scp173Anim.SetAnimationRange(1.0f, 30.0f);

    Shader font_shader = Shader("resources/glsl/Font_vs.txt", "resources/glsl/Font_fs.txt");
    font_shader.Link();
    font_shader.Use();
    font_shader.SetInt("text", 0);
    font_shader.SetVec3("textColor", VECTOR3(1.0f, 0.0f, 0.0f));
    font_shader.SetMatrix("projection", projection);

    Font text1 = Font("你好世界!", "C:/Windows/Fonts/simfang.ttf", 12);

    MATRIX font_model = IdentityMatrix();

    while(window.Run())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        Input_Update(window.window);
        
        scp173Anim.SetAnimation(mode, 1.0f);

        if(Input_IsKeyReleased(GLFW_KEY_ESCAPE))
            window.run = false;        

        if(Input_IsKeyReleased(GLFW_KEY_SPACE))
            mode = 0;         

        camera.Move(speed * deltaTime, 50.0f * deltaTime);                
        
        scp173Anim.UpdateAnimation(currentTime);

        renderer->Clear(0, 0, 0);

        shader.Use();
        shader.SetMatrix("view", camera.Matrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("nm", NM);

        shader.SetVec3("ViewPos", camera.Pos());

        shader.SetVec3("light.Position", pbr_light.Position);
        shader.SetVec3("light.Color", pbr_light.Color);

        scp173Anim.SetShader(shader);

        SetCubeTexture(ibl.irradianceMap, GL_TEXTURE0 + scp173.texCount);
        SetCubeTexture(ibl.prefilterMap, GL_TEXTURE0 + scp173.texCount + 1);
        SetTexture(ibl.brdfLUTTexture, GL_TEXTURE0 + scp173.texCount + 2);

        SetTexture(normal, GL_TEXTURE0 + scp173.texCount + 3);

        shader.SetMatrix("model", model);

        scp173.Draw(shader);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
        L_shader.Use();
        L_shader.SetMatrix("view", camera.Matrix());
        L_shader.SetMatrix("projection", projection);
                                
        L_shader.SetMatrix("model", L_model);
         
        LCube.Draw(L_shader);
        
////////////////////////////////////////////////////////////////////////////////////////////////////////        
        //font_model = BillboardMatrix(VECTOR3(0.0f, 1.0f, -3.0f), camera.Matrix());
        font_shader.Use();
        font_shader.SetMatrix("model", font_model);
        font_shader.SetMatrix("view", camera.Matrix());
        
        text1.Draw(0.0f, 0.0f, -1.0f , 0.01f);
////////////////////////////////////////////////////////////////////////////////////////

        FinalSkyBoxShader.Use();
        FinalSkyBoxShader.SetMatrix("view", camera.Matrix());
        FinalSkyBoxShader.SetMatrix("projection", projection);
    
        ibl.Draw(FinalSkyBox, FinalSkyBoxShader);

//////////////////////////////////////////////////////////////////////////////

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    scp173.Free();
    FreeTexture(normal);
    delete renderer;
    return 0;
}