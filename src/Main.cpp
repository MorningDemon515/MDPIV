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

using namespace MD_Math;

std::string title = "MDPIV";

unsigned int VBO, VAO, EBO ,LightVAO, t1, t2, n;
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
        
        renderer->Present(window.window);
        
        window.Quit();
    }
    
    FreeTexture(t1);
    FreeTexture(t2);
    FreeTexture(n);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &LightVAO);
    delete renderer;
    return 0;
}