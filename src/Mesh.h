#ifndef MESH_H
#define MESH_H

#include "Shader.h"

#include <iostream>
#include <string>
#include <vector>

struct Vertex
{
    MD_Math::VECTOR3 Position;
    MD_Math::VECTOR3 Normal;
    MD_Math::VECTOR2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    void Init();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    Mesh(){};
    Mesh(std::vector<Vertex> v ,std::vector<unsigned int> i, std::vector<Texture> t );
    ~Mesh();

    void Draw(Shader& Shader);
};

#endif 