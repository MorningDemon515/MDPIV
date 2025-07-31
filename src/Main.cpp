
#include "Window.h"
#include "OGL/Renderer_GL.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Materials.h"
#include "Light.h"
#include "LoadImage.h"

#include <iostream>
#include "stb_image.h"

#include "Model.h"

using namespace MD_Math;

std::string title = "MDPIV";

Mesh CreateCubeMesh() {
     std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    std::vector<Vertex> cubeVertices = {

        { VECTOR3(-0.5f, -0.5f,  0.5f), VECTOR3(0.0f, 0.0f, 1.0f), VECTOR2(0.0f, 0.0f) },
        { VECTOR3( 0.5f, -0.5f,  0.5f), VECTOR3(0.0f, 0.0f, 1.0f), VECTOR2(1.0f, 0.0f) },
        { VECTOR3( 0.5f,  0.5f,  0.5f), VECTOR3(0.0f, 0.0f, 1.0f), VECTOR2(1.0f, 1.0f) },
        { VECTOR3(-0.5f,  0.5f,  0.5f), VECTOR3(0.0f, 0.0f, 1.0f), VECTOR2(0.0f, 1.0f) },
        
        { VECTOR3(-0.5f, -0.5f, -0.5f), VECTOR3(0.0f, 0.0f, -1.0f), VECTOR2(1.0f, 0.0f) },
        { VECTOR3( 0.5f, -0.5f, -0.5f), VECTOR3(0.0f, 0.0f, -1.0f), VECTOR2(0.0f, 0.0f) },
        { VECTOR3( 0.5f,  0.5f, -0.5f), VECTOR3(0.0f, 0.0f, -1.0f), VECTOR2(0.0f, 1.0f) },
        { VECTOR3(-0.5f,  0.5f, -0.5f), VECTOR3(0.0f, 0.0f, -1.0f), VECTOR2(1.0f, 1.0f) },
        
        { VECTOR3(-0.5f, -0.5f, -0.5f), VECTOR3(-1.0f, 0.0f, 0.0f), VECTOR2(0.0f, 0.0f) },
        { VECTOR3(-0.5f, -0.5f,  0.5f), VECTOR3(-1.0f, 0.0f, 0.0f), VECTOR2(1.0f, 0.0f) },
        { VECTOR3(-0.5f,  0.5f,  0.5f), VECTOR3(-1.0f, 0.0f, 0.0f), VECTOR2(1.0f, 1.0f) },
        { VECTOR3(-0.5f,  0.5f, -0.5f), VECTOR3(-1.0f, 0.0f, 0.0f), VECTOR2(0.0f, 1.0f) },
        
        { VECTOR3(0.5f, -0.5f,  0.5f), VECTOR3(1.0f, 0.0f, 0.0f), VECTOR2(0.0f, 0.0f) },
        { VECTOR3(0.5f, -0.5f, -0.5f), VECTOR3(1.0f, 0.0f, 0.0f), VECTOR2(1.0f, 0.0f) },
        { VECTOR3(0.5f,  0.5f, -0.5f), VECTOR3(1.0f, 0.0f, 0.0f), VECTOR2(1.0f, 1.0f) },
        { VECTOR3(0.5f,  0.5f,  0.5f), VECTOR3(1.0f, 0.0f, 0.0f), VECTOR2(0.0f, 1.0f) },
        
        { VECTOR3(-0.5f, 0.5f,  0.5f), VECTOR3(0.0f, 1.0f, 0.0f), VECTOR2(0.0f, 0.0f) },
        { VECTOR3( 0.5f, 0.5f,  0.5f), VECTOR3(0.0f, 1.0f, 0.0f), VECTOR2(1.0f, 0.0f) },
        { VECTOR3( 0.5f, 0.5f, -0.5f), VECTOR3(0.0f, 1.0f, 0.0f), VECTOR2(1.0f, 1.0f) },
        { VECTOR3(-0.5f, 0.5f, -0.5f), VECTOR3(0.0f, 1.0f, 0.0f), VECTOR2(0.0f, 1.0f) },
        
        { VECTOR3(-0.5f, -0.5f, -0.5f), VECTOR3(0.0f, -1.0f, 0.0f), VECTOR2(0.0f, 0.0f) },
        { VECTOR3( 0.5f, -0.5f, -0.5f), VECTOR3(0.0f, -1.0f, 0.0f), VECTOR2(1.0f, 0.0f) },
        { VECTOR3( 0.5f, -0.5f,  0.5f), VECTOR3(0.0f, -1.0f, 0.0f), VECTOR2(1.0f, 1.0f) },
        { VECTOR3(-0.5f, -0.5f,  0.5f), VECTOR3(0.0f, -1.0f, 0.0f), VECTOR2(0.0f, 1.0f) }
    };
    
    std::vector<unsigned int> cubeIndices = {
        // 前面
        0, 1, 2,  2, 3, 0,
        // 后面
        4, 5, 6,  6, 7, 4,
        // 左面
        8, 9, 10, 10, 11, 8,
        // 右面
        12, 13, 14, 14, 15, 12,
        // 顶面
        16, 17, 18, 18, 19, 16,
        // 底面
        20, 21, 22, 22, 23, 20
    };
    
    return Mesh(cubeVertices, cubeIndices, {});
}

int main()
{
    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    Input_Init(window.window);
    
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    MATRIX model = IdentityMatrix();
    MATRIX view = ViewMatrixRH(
        VECTOR3(0.0f, 0.0f, 3.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 1.0f, 0.0)
    );
    MATRIX projection = PerspectiveMatrixRH(
        AngularToRadian(45.0f),
        (float)window.width / (float)window.height,
        0.1f,
        100.0f
    );
    MATRIX NM = NormalMatrix(model);
    
    Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");
    shader.Link();
    //Model scp173 = Model("resources/model/scp-173.obj");
    
    Mesh cubeMesh = CreateCubeMesh();
    
    Image image;
    Load_Image_GL("resources/image.jpg", &image);

    if (image.id == 0) {
        unsigned char defaultTexture[] = {
            255, 0, 255,  0, 255, 0,
            0, 255, 0,  255, 0, 255
        };
        
        glGenTextures(1, &image.id);
        glBindTexture(GL_TEXTURE_2D, image.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, defaultTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        std::cout << "使用默认纹理" << std::endl;
    }
    
    Texture texture;
    texture.id = image.id;
    texture.type = "texture_diffuse";
    texture.path = "resources/image.jpg";
    
    cubeMesh.textures.push_back(texture);

    Camera camera = Camera();

    static double lastTime = glfwGetTime();
    system("color a");
    while(window.Run())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastTime);
        lastTime = currentTime;

        Input_Update(window.window);

        if(Input_IsKeyReleased(GLFW_KEY_ESCAPE))
            window.run = false;        

        camera.Move(2.0f * deltaTime, 50.0f * deltaTime);

        renderer->Clear(30, 30, 30);

        shader.Use();
        shader.SetMatrix("model", model);
        shader.SetMatrix("view", camera.Matrix());
        shader.SetMatrix("projection", projection);
        shader.SetMatrix("nm", NM);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, image.id);
        shader.SetInt("texture_diffuse0", 0);
        //scp173.Draw(shader);
        cubeMesh.Draw(shader);

        renderer->Present(window.window);
        
        window.Quit();
    }
    
    delete renderer;
    return 0;
}