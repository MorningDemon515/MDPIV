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

using namespace MD_Math;

std::string title = "MDPIV";

unsigned int VBO, VAO, EBO ,LightVAO, t1, t2, n, FBO, texture_fbo, RBO, quadVAO, quadVBO, skyboxVAO, skyboxVBO;
Shader shader = Shader("resources/glsl/vertex.txt", "resources/glsl/fragment.txt");
Shader L_shader = Shader("resources/glsl/light_vertex.txt", "resources/glsl/light_fragment.txt");

Materials_Texture materials =
{
    32.0f
};

struct Vertex
{
    VECTOR3 Position;
    VECTOR3 Normals;
    VECTOR2 TexCoords;
    VECTOR3 Tangent;   
    VECTOR3 Bitangent; 
};

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

std::vector<Vertex> vertices;

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

void ComputeTangents(std::vector<Vertex>& vertices, 
                      const std::vector<unsigned int>& indices) {
    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i+1]];
        Vertex& v2 = vertices[indices[i+2]];
        
        VECTOR3 edge1 = v1.Position - v0.Position;
        VECTOR3 edge2 = v2.Position - v0.Position;
        
        VECTOR2 deltaUV1 = v1.TexCoords - v0.TexCoords;
        VECTOR2 deltaUV2 = v2.TexCoords - v0.TexCoords;
        
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        VECTOR3 tangent(0.0f, 0.0f, 0.0f);
        VECTOR3 bitangent(0.0f, 0.0f, 0.0f);
        
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        
        v0.Tangent += tangent;
        v1.Tangent += tangent;
        v2.Tangent += tangent;
        
        v0.Bitangent += bitangent;
        v1.Bitangent += bitangent;
        v2.Bitangent += bitangent;
    }
    
    for (auto& vertex : vertices) {
        vertex.Tangent = Vector3Normalized(vertex.Tangent);
        vertex.Bitangent = Vector3Normalized(vertex.Bitangent);
    }
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

 float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

int main()
{
    Vertex temp = {VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f)};
    VECTOR3 normal(0.0f, 0.0f, 0.0f);
    
    int j = 0, c = 0;
    for(int i = 0; i < 24; i++)
    {
        if(i % 4 == 0 && i != 0)
        {
            j += 1; 
            c += 4;
        }
            
        temp.Position = pos[i];
        normal = -Vector3Normalized(
            VectorCross(
                pos[c + 1] - pos[c],
                pos[c + 2] - pos[c]));

        temp.Normals = normal;
        temp.TexCoords = texc[i];
        vertices.push_back(temp);
    }

    ComputeTangents(vertices, indices);

    Window window = Window(800, 600);
    window.SetTitle(title.c_str());
    window.SetICON("resources/icon/MorningDemon.jpg");

    Renderer* renderer = new Renderer_GL();
    renderer->Init(window.window);

    Input_Init(window.window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normals));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.width, window.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_fbo, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.width, window.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> faces
{
    "resources/skybox/right.jpg",
    "resources/skybox/left.jpg",
    "resources/skybox/top.jpg",
    "resources/skybox/bottom.jpg",
    "resources/skybox/front.jpg",
    "resources/skybox/back.jpg"
};
unsigned int cubemapTexture = loadCubemap(faces);

    Shader skyboxshader("resources/glsl/Skybox_vs.txt","resources/glsl/Skybox_fs.txt");
    skyboxshader.Link();

    skyboxshader.Use();
    skyboxshader.SetInt("skybox", 0);

    MATRIX skyboxModel = ScaleMatrix(100.0f, 100.0f, 100.0f);

//////////////////////////////////////////////////////////////////////////////////////////////////////////    
    t1 = TextureFromFileRGB("resources/image.jpg");
    t2 = TextureFromFileRGBA("resources/image2.png");
    n = TextureFromFileRGB("resources/image_n.png");

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

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        renderer->Clear(0, 0, 0);
        glEnable(GL_DEPTH_TEST);

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

        shader.SetInt("texture_diffuse", 1);
        SetTexture(t1, GL_TEXTURE1);

        shader.SetInt("texture_specular", 0);
        SetTexture(t2, GL_TEXTURE0);

        shader.SetInt("texture_normal", 2);
        SetTexture(n, GL_TEXTURE2);

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
         
            model = TranslationMatrix(cube_Positions[i].x , cube_Positions[i].y, cube_Positions[i].z) * ScaleMatrix(0.5f, 0.5f, 0.5f);
            shader.SetMatrix("model", model);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);

        L_shader.Use();
        L_shader.SetMatrix("view", camera.Matrix());
        L_shader.SetMatrix("projection", projection);
        
        glBindVertexArray(LightVAO);
        for(unsigned int j = 0; j < 4; j++)
        {
            L_model = TranslationMatrix(pointLightPositions[j].x, pointLightPositions[j].y, pointLightPositions[j].z) * 
                     ScaleMatrix(0.2f, 0.2f, 0.2f) * ScaleMatrix(0.5f, 0.5f, 0.5f);

            L_shader.SetMatrix("model", L_model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);

//////////////////////////////////////////////////////////////////////////////
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        skyboxshader.Use();
        skyboxshader.SetMatrix("model", skyboxModel);
        skyboxshader.SetMatrix("view", camera.Matrix());
        skyboxshader.SetMatrix("projection", projection);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

/////////////////////////////////////////////////////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, 0); 
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        FBshader.Use();  
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, texture_fbo);
         glActiveTexture(GL_TEXTURE0); // 确保激活0号单元
         glBindTexture(GL_TEXTURE_2D, texture_fbo);

        glDrawArrays(GL_TRIANGLES, 0, 6);  
        
        renderer->Present(window.window);
        
        window.Quit();
    }
    
    FreeTexture(t1);
    FreeTexture(t2);
    FreeTexture(n);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteVertexArrays(1, &LightVAO);
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &texture_fbo);
    glDeleteRenderbuffers(1, &RBO);
    delete renderer;
    return 0;
}