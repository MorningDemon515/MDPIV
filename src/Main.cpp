
#include "Window.h"
#include "MD_Math.h"
#include "Camera.h"
#include "OGL/Renderer_GL.h"
#include "Model.h"
#include <iostream>
#include "stb_image.h"
#include "Font.h"
#include "Texture.h"

using namespace MD_Math;

std::string title = "MDPIV";

int main()
{
    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    Input_Init(window.window);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    stbi_set_flip_vertically_on_load(true);
///////////////////////////////////////////////////////////////////////////////////
    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        (float)window.width / (float)window.height,
        0.1f,
        100.0f
    );

    MATRIX projection2 = OrthoMatrixRH(
        0.0f, (float)window.width , 0.0f, (float)window.height, 0.1f, 100.0f
    );
    
    Model scp173 = Model("resources/model/scp173.fbx");
    Shader scp173Shader = Shader("resources/glsl/model_vs.txt", "resources/glsl/model_fs.txt");
    scp173Shader.Link();
    MATRIX scp173model =  TranslationMatrix(0.0f, 0.0f, 0.0f) * ScaleMatrix(0.001f, 0.001f, 0.001f);

    Quad plane = Quad();
    Shader planeShader = Shader("resources/glsl/plane_vs.txt", "resources/glsl/plane_fs.txt");
    planeShader.Link();
    MATRIX planemodel = TranslationMatrix(0.0f, -0.035f, 0.0f) *
                        RotationMatrix(AngularToRadian(-90.0f), 'X') *
                        ScaleMatrix(10.0f, 10.0f, 10.0f);            

    Camera camera = Camera();
    camera.SetPos(VECTOR3(0.0f, 0.0f, 13.0f));         

    Font text1 = Font("For B3D!", "C:/Windows/Fonts/simfang.ttf", 16);
    Shader text1Shader = Shader("resources/glsl/Font_vs.txt", "resources/glsl/Font_fs.txt");
    text1Shader.Link();
    text1Shader.Use();
    text1Shader.SetMatrix("projection", projection);
    text1Shader.SetMatrix("view", ViewMatrixRH(VECTOR3(0.0f, 0.0f, 3.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f)));
    text1Shader.SetMatrix("model", IdentityMatrix());
    text1Shader.SetVec3("textColor", VECTOR3(1.0f, 1.0f, 1.0f));

    Cube lightCube = Cube();
    Shader lightShader = Shader("resources/glsl/light_vertex.txt", "resources/glsl/light_fragment.txt");
    lightShader.Link();
    MATRIX lightCubeModel = TranslationMatrix(2.0f, 2.0f, 2.0f) * ScaleMatrix(0.2f, 0.2f, 0.2f);

    VECTOR3 LightPos = VECTOR3(2.0f, 2.0f, 2.0f);

    unsigned int depthMapFBO;
    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    unsigned int depthMap;
    Shader shadowShader = Shader("resources/glsl/shadow_vs.txt", "resources/glsl/shadow_fs.txt");
    MATRIX lightSpaceMatrix;

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shadowShader.Link();
    
///////////////////////////////////////////////////////////////////////////////////
    system("color a");
    static double lastTime = glfwGetTime();
    float speed = 3.0f;
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

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        MATRIX lightProjection = OrthoMatrixRH(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
        MATRIX lightView = ViewMatrixRH(LightPos, VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;

        shadowShader.Use();
        shadowShader.SetMatrix("lightSpaceMatrix", lightSpaceMatrix);

        shadowShader.SetMatrix("model", scp173model);
        scp173.Draw(shadowShader);

        shadowShader.SetMatrix("model", planemodel);
        plane.Draw(shadowShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.width, window.height);

        renderer->Clear(0, 0, 0);

        text1Shader.Use();
        text1.Draw(-0.5f, 0.8f, 0.0f, 0.003f);

        lightShader.Use();
        lightShader.SetMatrix("projection", projection);
        lightShader.SetMatrix("view", camera.Matrix());
        lightShader.SetMatrix("model", lightCubeModel);
        lightCube.Draw(lightShader);

        SetTexture(depthMap, GL_TEXTURE1);
        
        planeShader.Use();
        planeShader.SetMatrix("projection", projection);
        planeShader.SetMatrix("view", camera.Matrix());
        planeShader.SetMatrix("model", planemodel);
        planeShader.SetMatrix("lightSpaceMatrix", lightSpaceMatrix);
        planeShader.SetVec3("ViewPos", camera.Pos());
        planeShader.SetVec3("LightPos", LightPos);
        planeShader.SetInt("shadowMap", 1);
        plane.Draw(planeShader);

        scp173Shader.Use();
        scp173Shader.SetMatrix("projection", projection);
        scp173Shader.SetMatrix("view", camera.Matrix());
        scp173Shader.SetMatrix("model", scp173model);
        scp173Shader.SetMatrix("lightSpaceMatrix", lightSpaceMatrix);
        scp173Shader.SetVec3("ViewPos", camera.Pos());
        scp173Shader.SetVec3("LightPos", LightPos);
        scp173Shader.SetInt("shadowMap", 1);
        scp173.Draw(scp173Shader);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    scp173.Free();
    delete renderer;
    return 0;
}


/*
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

    unsigned int FBO, texture_fbo, RBO;
    unsigned int quadVAO, quadVBO;

    Shader FBshader = Shader("resources/glsl/FB_vs.txt","resources/glsl/FB_fs.txt");	
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

    FBshader.Link();

    FBshader.Use();
    FBshader.SetInt("screenTexture", 0);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &texture_fbo);
    glBindTexture(GL_TEXTURE_2D, texture_fbo);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.width, window.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_fbo, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.width, window.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

    Quad plane = Quad();
    Shader planeShader = Shader("resources/glsl/plane_vs.txt", "resources/glsl/plane_fs.txt");
    planeShader.Link();
    MATRIX planemodel = TranslationMatrix(0.0f, -1.0f, 0.0f) *
                        RotationMatrix(AngularToRadian(-90.0f), 'X') *
                        ScaleMatrix(10.0f, 10.0f, 10.0f);

    MATRIX p_NM = NormalMatrix(planemodel);               

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

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        renderer->Clear(0, 0, 0);
		glEnable(GL_DEPTH_TEST);

        planeShader.Use();
    
        planeShader.SetMatrix("projection", projection);
        planeShader.SetMatrix("view", camera.Matrix());
        planeShader.SetMatrix("model", planemodel);

        planeShader.SetMatrix("nm", p_NM);

        planeShader.SetVec3("ViewPos", camera.Pos());

        planeShader.SetVec3("light.Position", pbr_light.Position);
        planeShader.SetVec3("light.Color", pbr_light.Color);

        planeShader.SetInt("irradianceMap", 1);
        planeShader.SetInt("prefilterMap", 2);
        planeShader.SetInt("brdfLUT", 3);     

        SetCubeTexture(ibl.irradianceMap, GL_TEXTURE0 + 1);
        SetCubeTexture(ibl.prefilterMap, GL_TEXTURE0 + 2);
        SetTexture(ibl.brdfLUTTexture, GL_TEXTURE0 + 3);

       plane.Draw(planeShader);

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
        
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		FBshader.Use();
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texture_fbo);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_fbo);

		glDrawArrays(GL_TRIANGLES, 0, 6);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    scp173.Free();
    FreeTexture(normal);
	
	glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &texture_fbo);
    glDeleteRenderbuffers(1, &RBO);

    delete renderer;
    return 0;
}
*/
